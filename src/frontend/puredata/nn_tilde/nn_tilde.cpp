
#include <stdio.h>
#include <stdlib.h>

#include <limits.h>
#include <memory>
#include <mutex>
#include <exception>
#include <string>
#include <vector>

#ifndef VERSION
#define VERSION "UNDEFINED"
#endif

#include "m_pd.h"
#ifndef CLASS_MULTICHANNEL
#if PD_MINOR_VERSION >= 54
#define PD_HAVE_MULTICHANNEL
#else
#pragma message("building without multi-channel support; requires Pd 0.54+")
#define CLASS_MULTICHANNEL 0
#endif
#else
#define PD_HAVE_MULTICHANNEL CLASS_MULTICHANNEL
#endif

#if defined(__APPLE__) && defined(__MACH__)
#include <mach-o/dyld.h>
#endif

#ifdef __unix__
#include <unistd.h>
#endif


#include "../../../backend/backend.h"
#include "../../../shared/circular_buffer.h"
#include "../shared/pd_model_download.h"
#include "../shared/pd_buffer_manager.h"

#ifdef _WIN32
  #define NOMINMAX
  #include <windows.h>
  #include <shlwapi.h>

  std::string get_executable_path()
  {
    HMODULE hModule = GetModuleHandle("nn~.dll");
    if (hModule)
    {
      char path[MAX_PATH];
      GetModuleFileName(hModule, path, sizeof(path));
      PathRemoveFileSpec(path); // Remove filename, leaving the path
      SetDllDirectory(path);    // Add to DLL search path
      
      std::string path_std = path;
      return path_std;
    }
  }
#else
  #include <dlfcn.h>
#endif


#if defined(__APPLE__) && defined(__MACH__)
#include <cstdint>
std::string get_executable_path()
{
  char buf[PATH_MAX];
  uint32_t bufsize = PATH_MAX;
  if (!_NSGetExecutablePath(buf, &bufsize))
    puts(buf);
  std::string path_std = buf;
  return path_std;
}
#endif
#if defined(__unix__)
std::string get_executable_path()
{
  char path[PATH_MAX];
  ssize_t count = readlink("/proc/self/exe", path, PATH_MAX - 1);
  if (count != -1)
  {
    path[count] = '\0'; // Null-terminate the string
    std::string path_std = path;
  }
  else
  {
    throw "could not extract file position";
  }
  return path;
}
#endif


using t_signal_setmultiout = void (*)(t_signal **, int);
static t_signal_setmultiout g_signal_setmultiout;
static t_class *nn_tilde_class;

unsigned power_ceil(unsigned x)
{
  if (x <= 1)
    return 1;
  int power = 2;
  x--;
  while (x >>= 1)
    power <<= 1;
  return power;
}

// CLASS LIKE INITIALISATION
typedef struct _nn_tilde
{
  t_object x_obj;
  t_sample f;
  bool m_multichannel;
  bool m_gpu;
  bool m_dims_changed;
  t_canvas *m_canvas;
  t_outlet *m_info_outlet;

  bool m_enabled;
  // BACKEND RELATED MEMBERS
  std::unique_ptr<Backend> m_model;
  std::unique_ptr<PdBufferManager<_nn_tilde>> m_buf_manager;
  std::vector<float *> m_in_model_ptrs;
  std::vector<float *> m_out_model_ptrs;
  std::vector<std::string> settable_attributes;
  t_symbol *m_method, *m_path;
  std::unique_ptr<std::thread> m_compute_thread;

  // BUFFER RELATED MEMBERS
  int m_head, m_in_dim, m_in_ratio, m_out_dim, m_out_ratio, m_buffer_size;

  std::unique_ptr<circular_buffer<float, float>[]> m_in_buffer;
  std::unique_ptr<circular_buffer<float, float>[]> m_out_buffer;
  std::vector<std::unique_ptr<float[]>> m_in_model, m_out_model;

  bool m_use_thread;

  // DSP RELATED MEMBERS
  int m_dsp_vec_size;
  std::vector<float *> m_dsp_in_vec;
  std::vector<float *> m_dsp_out_vec;

  // DOWNLOAD RELATED MEMBERS
  std::unique_ptr<PdModelDownloader<_nn_tilde>> m_downloader;

} t_nn_tilde;

class PdErrorCatcher
{
  std::streambuf *old_cerr;
  std::stringstream error_stream;
  t_nn_tilde *x; // Store Pd object pointer

public:
  PdErrorCatcher(t_nn_tilde *obj) : x(obj)
  {
    old_cerr = std::cerr.rdbuf(error_stream.rdbuf());
  }

  ~PdErrorCatcher()
  {
    // Output any caught errors to Pd console
    std::string errors = error_stream.str();
    if (!errors.empty())
    {
      pd_error(x, "nn~: %s", errors.c_str());
    }
    // Restore original stderr
    std::cerr.rdbuf(old_cerr);
  }
};

void model_perform(t_nn_tilde *x)
{
  PdErrorCatcher error_catcher(x);
  try
  {
    x->m_model->perform(x->m_in_model_ptrs, x->m_out_model_ptrs,
                        x->m_method->s_name, 1, x->m_out_dim, x->m_buffer_size);
  }
  catch (const std::exception &e)
  {
    pd_error(x, "nn~: model perform error: %s", e.what());
  }
}

// DSP CALL
t_int *nn_tilde_perform(t_int *w)
{
  t_nn_tilde *x = (t_nn_tilde *)(w[1]);

  if (!x->m_model->is_loaded() || !x->m_enabled)
  {
    for (int c(0); c < x->m_out_dim; c++)
    {
      float *out = x->m_dsp_out_vec[c];
      memset(out, 0, x->m_dsp_vec_size * sizeof(float));
    }
    return w + 2;
  }

  // COPY INPUT TO CIRCULAR BUFFER
  for (int c(0); c < x->m_in_dim; c++)
    x->m_in_buffer[c].put(x->m_dsp_in_vec[c], x->m_dsp_vec_size);

  if (x->m_in_buffer[0].full())
  { // BUFFER IS FULL
    // IF USE THREAD, CHECK THAT COMPUTATION IS OVER
    if (x->m_compute_thread && x->m_use_thread)
      x->m_compute_thread->join();

    // TRANSFER MEMORY BETWEEN INPUT CIRCULAR BUFFER AND MODEL BUFFER
    for (int c(0); c < x->m_in_dim; c++)
      x->m_in_buffer[c].get(x->m_in_model[c].get(), x->m_buffer_size);

    if (!x->m_use_thread) // PROCESS DATA RIGHT NOW
      model_perform(x);

    // TRANSFER MEMORY BETWEEN OUTPUT CIRCULAR BUFFER AND MODEL BUFFER
    for (int c(0); c < x->m_out_dim; c++)
      x->m_out_buffer[c].put(x->m_out_model[c].get(), x->m_buffer_size);

    if (x->m_use_thread) // PROCESS DATA LATER
      x->m_compute_thread = std::make_unique<std::thread>(model_perform, x);
  }

  // COPY CIRCULAR BUFFER TO OUTPUT
  for (int c(0); c < x->m_out_dim; c++)
    x->m_out_buffer[c].get(x->m_dsp_out_vec[c], x->m_dsp_vec_size);

  return w + 2;
}

void nn_tilde_dsp(t_nn_tilde *x, t_signal **sp)
{
  x->m_dsp_vec_size = sp[0]->s_n;
  x->m_dsp_in_vec.clear();
  x->m_dsp_out_vec.clear();

  if (x->m_multichannel)
  {
// Get number of available input channels
#if PD_HAVE_MULTICHANNEL
    int nchans = sp[0]->s_nchans;
#else
    int nchans = 1;
#endif

    // Map input channels, wrapping if needed
    for (int i(0); i < x->m_in_dim; i++)
      x->m_dsp_in_vec.push_back(sp[0]->s_vec + x->m_dsp_vec_size * (i % nchans));

    // Configure multichannel output
    g_signal_setmultiout(&sp[1], x->m_out_dim);
    for (int i(0); i < x->m_out_dim; i++)
      x->m_dsp_out_vec.push_back(sp[1]->s_vec + x->m_dsp_vec_size * i);
  }
  else
  {
    // Standard mode - separate signals
    for (int i(0); i < x->m_in_dim; i++)
      x->m_dsp_in_vec.push_back(sp[i]->s_vec);
    for (int i(x->m_in_dim); i < x->m_in_dim + x->m_out_dim; i++)
    {
      if (g_signal_setmultiout)
        g_signal_setmultiout(&sp[i], 1); // Ensure single channel
      x->m_dsp_out_vec.push_back(sp[i]->s_vec);
    }
  }
  dsp_add(nn_tilde_perform, 1, x);
}

void nn_tilde_free(t_nn_tilde *x)
{
  if (x->m_compute_thread)
    x->m_compute_thread->join();
}

std::string resolve_file_path(t_nn_tilde *x, const char *filename)
{
  char dirresult[MAXPDSTRING];
  char *nameresult;
  int fd;

  // Add .ts extension if not present
  std::string fname(filename);
  if (fname.substr(fname.length() - 3) != ".ts")
    fname += ".ts";

  const char *canvas_dir = canvas_getdir(x->m_canvas)->s_name;

  // Try to open from canvas dir first, then search other paths
  fd = open_via_path(canvas_dir, fname.c_str(), "", dirresult, &nameresult, MAXPDSTRING, 1);

  if (fd >= 0)
  {
    sys_close(fd);
    std::filesystem::path fullpath = dirresult;
    fullpath /= nameresult;
    // char fullpath[MAXPDSTRING];
    // std::ostringstream str;
    // str <<fullpath << std::to_string(MAXPDSTRING).c_str(), "%s/%s", dirresult, nameresult);
    char normalized[MAXPDSTRING];
    sys_unbashfilename(fullpath.string().c_str(), normalized);
    return std::string(normalized);
  }

  pd_error(x, "nn~: could not find file '%s'", fname.c_str());
  return "";
}

void create_buffers(t_nn_tilde *x, int in_dim, int out_dim)
{
  x->m_in_buffer = std::make_unique<circular_buffer<float, float>[]>(in_dim);
  x->m_out_buffer = std::make_unique<circular_buffer<float, float>[]>(out_dim);

  x->m_in_model.clear();
  x->m_out_model.clear();

  for (int i(0); i < in_dim; i++)
  {
    x->m_in_buffer[i].initialize(x->m_buffer_size);
    x->m_in_model.push_back(std::make_unique<float[]>(x->m_buffer_size));
  }

  for (int i(0); i < out_dim; i++)
  {
    x->m_out_buffer[i].initialize(x->m_buffer_size);
    x->m_out_model.push_back(std::make_unique<float[]>(x->m_buffer_size));
  }

  // Pre-allocate pointer vectors
  x->m_in_model_ptrs.clear();
  x->m_out_model_ptrs.clear();
  x->m_in_model_ptrs.reserve(in_dim);
  x->m_out_model_ptrs.reserve(out_dim);

  for (int i(0); i < in_dim; i++)
    x->m_in_model_ptrs.push_back(x->m_in_model[i].get());
  for (int i(0); i < out_dim; i++)
    x->m_out_model_ptrs.push_back(x->m_out_model[i].get());
}

bool nn_tilde_update_model_params(t_nn_tilde *x, t_symbol *method)
{
  x->m_method = method;

  // Get the method parameters
  auto params = x->m_model->get_method_params(x->m_method->s_name);

  // Try to fallback to forward method if method not found
  if (!params.size())
  {
    post("nn~: method %s not found in model, using forward instead", x->m_method->s_name);
    x->m_method = gensym("forward");
    params = x->m_model->get_method_params(x->m_method->s_name);
    if (!params.size())
    {
      pd_error(x, "nn~: forward method not found in model");
      return false;
    }
  }

  // Store old dimensions to check if they changed
  int old_in_dim = x->m_in_dim;
  int old_out_dim = x->m_out_dim;
  x->m_dims_changed = false;

  // Update dimensions and ratios
  x->m_in_dim = params[0];
  x->m_in_ratio = params[1];
  x->m_out_dim = params[2];
  x->m_out_ratio = params[3];

  // Check/adjust buffer size
  auto higher_ratio = x->m_model->get_higher_ratio();
  if (!x->m_buffer_size)
  {
    // NO THREAD MODE
    x->m_use_thread = false;
    x->m_buffer_size = higher_ratio;
  }
  else if (x->m_buffer_size < higher_ratio)
  {
    x->m_buffer_size = higher_ratio;
    post("nn~: buffer size too small, switching to %d", x->m_buffer_size);
  }
  else
  {
    x->m_buffer_size = power_ceil(x->m_buffer_size);
  }

  // Create new buffers if dimensions changed
  x->m_dims_changed = (x->m_in_dim != old_in_dim) || (x->m_out_dim != old_out_dim);
  if (x->m_dims_changed)
  {
    create_buffers(x, x->m_in_dim, x->m_out_dim);
    post("nn~: dimensions changed: %d in, %d out", x->m_in_dim, x->m_out_dim);
  }

  return true;
}

// MODEL LOADER
bool nn_tilde_load_model(t_nn_tilde *x, const char *path)
{
  // Resolve the file path
  std::string fullpath = resolve_file_path(x, path);
  if (fullpath.empty())
    return false;

  // Create and load new backend instance
  auto new_model = std::make_unique<Backend>();
  if (new_model->load(fullpath.c_str(), (double)sys_getsr()))
  {
    pd_error(x, "error loading model %s", path);
    return false;
  }

  // Store the new model and path
  x->m_model = std::move(new_model);
  x->m_path = gensym(fullpath.c_str());
  x->settable_attributes = x->m_model->get_settable_attributes();

  if (x->m_gpu && !(torch::hasCUDA() || torch::hasMPS()))
  {
    post("nn~: GPU mode not available");
    x->m_gpu = 0;
  }
  else if (x->m_gpu)
  {
    const char *gputype = torch::hasCUDA() ? "CUDA" : "MPS";
    post("nn~: %s found", gputype);
  }

  if (x->m_gpu)
    x->m_model->use_gpu(true);
  try
  {
    x->m_buf_manager->init_buffer_list(x->m_model.get());
  }
  catch (std::string &e)
  {
    pd_error(x, "nn~: %s", e.c_str());
  }

  // Update parameters using current method (or fallback to forward)
  return nn_tilde_update_model_params(x, x->m_method);
}

void nn_tilde_bang(t_nn_tilde *x)
{
  // Output "is_loaded" status
  t_atom is_loaded;
  SETFLOAT(&is_loaded, x->m_model->is_loaded());
  outlet_anything(x->m_info_outlet, gensym("loaded"), 1, &is_loaded);

  // Return if no model is loaded
  if (!x->m_model->is_loaded())
    return;

  // Output "enabled" status
  t_atom enabled;
  SETFLOAT(&enabled, x->m_enabled);
  outlet_anything(x->m_info_outlet, gensym("enabled"), 1, &enabled);

  // Output "gpu" status
  t_atom gpu;
  SETFLOAT(&gpu, x->m_gpu);
  outlet_anything(x->m_info_outlet, gensym("gpu"), 1, &gpu);

  // Output model path
  t_atom model;
  SETSYMBOL(&model, x->m_path);
  outlet_anything(x->m_info_outlet, gensym("model"), 1, &model);

  // Output modes
  std::vector<std::string> methods = x->m_model->get_available_methods();
  std::vector<t_atom> method_atoms(methods.size());
  for (size_t i = 0; i < methods.size(); i++)
    SETSYMBOL(&method_atoms[i], gensym(methods[i].c_str()));
  outlet_anything(x->m_info_outlet, gensym("methods"),
                  methods.size(), method_atoms.data());

  // Output selected method
  t_atom method;
  SETSYMBOL(&method, x->m_method);
  outlet_anything(x->m_info_outlet, gensym("method"), 1, &method);

  if (x->settable_attributes.empty())
  {
    // Output empty symbol when no attributes
    t_atom empty;
    SETSYMBOL(&empty, gensym("")); // or you could use gensym("none")
    outlet_anything(x->m_info_outlet, gensym("attributes"), 1, &empty);
  }
  else
  {
    std::vector<t_atom> attr_atoms(x->settable_attributes.size());
    for (size_t i = 0; i < x->settable_attributes.size(); i++)
    {
      SETSYMBOL(&attr_atoms[i], gensym(x->settable_attributes[i].c_str()));
    }
    outlet_anything(x->m_info_outlet, gensym("attributes"),
                    attr_atoms.size(), attr_atoms.data());
  }
  // Output buffer size
  t_atom bufsize;
  SETFLOAT(&bufsize, x->m_buffer_size);
  outlet_anything(x->m_info_outlet, gensym("bufsize"), 1, &bufsize);

  // Output dimensions
  t_atom dims[2];
  SETFLOAT(dims, x->m_in_dim);
  SETFLOAT(dims + 1, x->m_out_dim);
  outlet_anything(x->m_info_outlet, gensym("dim"), 2, dims);

  // Output ratios
  t_atom ratios[2];
  SETFLOAT(ratios, x->m_in_ratio);
  SETFLOAT(ratios + 1, x->m_out_ratio);
  outlet_anything(x->m_info_outlet, gensym("ratio"), 2, ratios);
}

void *nn_tilde_new(t_symbol *s, int argc, t_atom *argv)
{
  t_nn_tilde *x = (t_nn_tilde *)pd_new(nn_tilde_class);

  x->m_model = std::make_unique<Backend>();
  x->m_buf_manager = std::make_unique<PdBufferManager<_nn_tilde>>(x->m_model.get(), x);
  x->m_head = 0;
  x->m_compute_thread = nullptr;
  x->m_in_dim = 1;
  x->m_in_ratio = 1;
  x->m_out_dim = 1;
  x->m_out_ratio = 1;
  x->m_buffer_size = 4096;
  x->m_method = gensym("forward");
  x->m_enabled = true;
  x->m_use_thread = true;
  x->m_multichannel = false;
  x->m_gpu = false;
  x->m_dims_changed = true;
  x->m_canvas = canvas_getcurrent();

  // init downloader
  auto canvas_dir = canvas_getdir(x->m_canvas)->s_name;
  x->m_downloader = std::make_unique<PdModelDownloader<_nn_tilde>>(x, canvas_dir);
  // #endif

  // Create minimum outlet (we already have main inlet from CLASS_MAINSIGNALIN)
  outlet_new(&x->x_obj, &s_signal);
  create_buffers(x, 1, 1); // minimum buffers for one in/out

  // CHECK ARGUMENTS
  // First check for optional flags
  while (argc > 0 && argv->a_type == A_SYMBOL)
  {
    t_symbol *flag = atom_getsymbol(argv);

    if (flag == gensym("-m"))
    {
      if (g_signal_setmultiout)
      {
        x->m_multichannel = true;
      }
      else
      {
        int maj = 0, min = 0, bug = 0;
        sys_getversion(&maj, &min, &bug);
        pd_error(x, "nn~: no multichannel support in Pd %i.%i-%i, ignoring '-m' flag",
                 maj, min, bug);
      }
      argc--;
      argv++;
    }
    else if (flag == gensym("-gpu") || flag == gensym("-g"))
    { // activate gpu mode
      x->m_gpu = true;
      argc--;
      argv++;
    }
    else if (flag == gensym("-d"))
    { // start in disabled mode
      x->m_enabled = false;
      argc--;
      argv++;
    }
    else
      break; // Not a flag, should be the model path
  }

  if (!argc)
    return (void *)x;

  // Process remaining regular arguments
  x->m_path = atom_getsymbol(argv);
  if (argc > 1)
    x->m_method = atom_gensym(argv + 1);
  if (argc > 2)
    x->m_buffer_size = atom_getint(argv + 2);

  if (nn_tilde_load_model(x, x->m_path->s_name))
  {
    // Add any additional inlets/outlets needed for model
    if (!x->m_multichannel)
    {
      for (int i(1); i < x->m_in_dim; i++)
        inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);
      for (int i(1); i < x->m_out_dim; i++)
        outlet_new(&x->x_obj, &s_signal);
    }
  }
  x->m_info_outlet = outlet_new(&x->x_obj, &s_anything);

  return (void *)x;
}

void nn_tilde_method(t_nn_tilde *x, t_symbol *s)
{
  if (!x->m_multichannel)
  {
    pd_error(x, "nn~: method change is only supported in multichannel mode");
    return;
  }

  if (x->m_compute_thread)
  {
    x->m_compute_thread->join();
    x->m_compute_thread = nullptr;
  }

  if (nn_tilde_update_model_params(x, s) && x->m_dims_changed)
    canvas_update_dsp();
}

void nn_tilde_bufsize(t_nn_tilde *x, t_floatarg size)
{
  if (x->m_compute_thread)
  {
    x->m_compute_thread->join();
    x->m_compute_thread = nullptr;
  }

  // Store and validate new size
  x->m_buffer_size = (int)size;
  auto higher_ratio = x->m_model->get_higher_ratio();
  if (!x->m_buffer_size)
  {
    // NO THREAD MODE
    x->m_use_thread = false;
    x->m_buffer_size = higher_ratio;
  }
  else if (x->m_buffer_size < higher_ratio)
  {
    x->m_buffer_size = higher_ratio;
    post("nn~: buffer size too small, switching to %d", x->m_buffer_size);
  }
  else
  {
    x->m_buffer_size = power_ceil(x->m_buffer_size);
  }

  // Recreate buffers with new size
  create_buffers(x, x->m_in_dim, x->m_out_dim);
}

void nn_tilde_load(t_nn_tilde *x, t_symbol *s)
{
  if (!x->m_multichannel)
  {
    pd_error(x, "nn~: dynamically loading models is only supported in multichannel mode");
    return;
  }

  if (x->m_compute_thread)
  {
    x->m_compute_thread->join();
    x->m_compute_thread = nullptr;
  }

  if (nn_tilde_load_model(x, s->s_name))
  {
    if (x->m_dims_changed)
    {
      canvas_update_dsp();
    }
  }
}

void nn_tilde_gpu(t_nn_tilde *x, t_floatarg arg)
{
  bool want_gpu = (bool)arg;
  if (want_gpu == x->m_gpu)
    return;

  if (!x->m_model->is_loaded())
  {
    pd_error(x, "nn~: no model loaded");
    return;
  }

  if (want_gpu && !(torch::hasCUDA() || torch::hasMPS()))
  {
    post("nn~: GPU mode not available");
    x->m_gpu = false;
    return;
  }

  x->m_gpu = want_gpu;
  x->m_model->use_gpu(want_gpu);
}

void nn_tilde_enable(t_nn_tilde *x, t_floatarg arg) { x->m_enabled = (bool)arg; }
void nn_tilde_reload(t_nn_tilde *x) { x->m_model->reload(); }

void nn_tilde_set(t_nn_tilde *x, t_symbol *s, int argc, t_atom *argv)
{
  if (argc < 2)
  {
    pd_error(x, "nn~: set needs at least 2 arguments [set argname argval1 ...(");
    return;
  }
  std::vector<std::string> attribute_args;

  auto argname = argv[0].a_w.w_symbol->s_name;
  std::string argname_str = argname;

  if (!std::count(x->settable_attributes.begin(), x->settable_attributes.end(),
                  argname_str))
  {
    pd_error(x, "nn~: argument name not settable in current model");
    return;
  }
  auto buffers = Backend::BufferMap();

  for (int i(1); i < argc; i++)
  {
    if (argv[i].a_type == A_SYMBOL)
    {
      attribute_args.push_back(argv[i].a_w.w_symbol->s_name);
      try
      {
        x->m_buf_manager->append_if_buffer_element<Backend>(buffers, argv[i].a_w.w_symbol->s_name, argname, i - 1);
      }
      catch (std::string &e)
      {
        pd_error(x, "%s", e.c_str());
      }
      catch (const std::exception &e)
      {
        pd_error(x, "nn~: %s", e.what());
      }
    }
    else if (argv[i].a_type == A_FLOAT)
    {
      attribute_args.push_back(std::to_string(argv[i].a_w.w_float));
    }
  }
  try
  {
    x->m_model->set_attribute(argname, attribute_args, buffers);
  }
  catch (const std::string &e)
  {
    pd_error(x, "nn~: %s", e.c_str());
  }
  catch (const std::exception &e)
  {
    pd_error(x, "nn~: %s", e.what());
  }
}

void nn_tilde_available_models(t_nn_tilde *x)
{
  try
  {
    if (x->m_downloader->is_ready())
    {
      auto available_models = x->m_downloader->get_available_models();
      for (auto model : available_models)
      {
        post(model.c_str());
      }
      return;
    }
    else
    {
      throw "download could be initailized";
    }
  }
  catch (std::string &e)
  {
    pd_error(x, "nn~: %s", e.c_str());
  }
  catch (std::exception &e)
  {
    pd_error(x, "nn~: %s", e.what());
  }
  return;
}

void nn_tilde_download(t_nn_tilde *x, t_symbol *s, int argc, t_atom *argv)
{
  if (argc == 0)
  {
    pd_error(x, "nn~: %s", "download needs at least a model card.");
    return;
  }
  if (argv[0].a_type != A_SYMBOL)
  {
    pd_error(x, "nn~: %s", "download takes a symbol as a model card.");
    return;
  }
  std::string model_name = argv[0].a_w.w_symbol->s_name;
  std::string rename_model = "";
  if (argc == 2)
  {
    if (argv[1].a_type != A_SYMBOL)
    {
      pd_error(x, "nn~: %s", "download takes a symbol as a model card, and an optional symbol as a custom name.");
      return;
    }
    rename_model = argv[1].a_w.w_symbol->s_name;
  }

  try
  {
    if (x->m_downloader->is_ready())
    {
      x->m_downloader->download(model_name, rename_model);
    }
  }
  catch (std::string &e)
  {
    pd_error(x, "nn~: %s", e.c_str());
  }
  catch (std::exception &e)
  {
    pd_error(x, "nn~: %s", e.what());
  }
}

void nn_tilde_remove(t_nn_tilde *x, t_symbol *s)
{
  try
  {
    std::string model_name = s->s_name;
    x->m_downloader->remove(model_name);
  }
  catch (std::string &e)
  {
    pd_error(x, "nn~: %s", e.c_str());
  }
  catch (std::exception &e)
  {
    pd_error(x, "nn~: %s", e.what());
  }
}

void startup_message()
{
  std::string startmessage = "nn~ - ";
  startmessage += VERSION;
  startmessage += " - ";
  startmessage += "torch ";
  startmessage += TORCH_VERSION;
  startmessage += " - 2025 - Antoine Caillon, Axel Chemla--Romeu-Santos, Benjamin Wesch";
  post(startmessage.c_str());
}

#ifdef _WIN32
#define EXPORT extern "C" __declspec(dllexport)
#elif __GNUC__ >= 4
#define EXPORT extern "C" __attribute__((visibility("default")))
#else
#define EXPORT extern "C"
#endif

EXPORT void nn_tilde_setup(void)
{

  // multichannel handling copied from
  // https://github.com/Spacechild1/vstplugin/blob/v0.6.0/pd/src/vstplugin~.cpp#L4120
#ifdef PD_HAVE_MULTICHANNEL
// runtime check for multichannel support:
#ifdef _WIN32
  // get a handle to the module containing the Pd API functions.
  // NB: GetModuleHandle("pd.dll") does not cover all cases.
  HMODULE module;
  if (GetModuleHandleEx(
          GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
          (LPCSTR)&pd_typedmess, &module))
  {
    g_signal_setmultiout = (t_signal_setmultiout)(void *)GetProcAddress(
        module, "signal_setmultiout");
  }
#else
  // search recursively, starting from the main program
  g_signal_setmultiout = (t_signal_setmultiout)dlsym(
      dlopen(nullptr, RTLD_NOW), "signal_setmultiout");
#endif
#endif // PD_HAVE_MULTICHANNEL

// Handle path for dynamically loaded cudnn_graph64_9.dll
// FIXME: is this really the way to do this??
#ifdef _WIN32
  // Get the directory of the current DLL
  HMODULE hModule = GetModuleHandle("nn~.dll");
  if (hModule)
  {
    char path[MAX_PATH];
    GetModuleFileName(hModule, path, sizeof(path));
    PathRemoveFileSpec(path); // Remove filename, leaving the path
    SetDllDirectory(path);    // Add to DLL search path
  }
#endif

  startup_message();
  nn_tilde_class = class_new(gensym("nn~"), (t_newmethod)nn_tilde_new, 0,
                             sizeof(t_nn_tilde), CLASS_MULTICHANNEL, A_GIMME, 0);
  class_addmethod(nn_tilde_class, (t_method)nn_tilde_dsp, gensym("dsp"), A_CANT, 0);
  class_addmethod(nn_tilde_class, (t_method)nn_tilde_enable, gensym("enable"), A_FLOAT, 0);
  class_addmethod(nn_tilde_class, (t_method)nn_tilde_load, gensym("load"), A_SYMBOL, 0);
  class_addmethod(nn_tilde_class, (t_method)nn_tilde_reload, gensym("reload"), A_NULL);
  class_addmethod(nn_tilde_class, (t_method)nn_tilde_set, gensym("set"), A_GIMME, 0);
  class_addmethod(nn_tilde_class, (t_method)nn_tilde_bufsize, gensym("bufsize"), A_FLOAT, 0);
  class_addmethod(nn_tilde_class, (t_method)nn_tilde_gpu, gensym("gpu"), A_FLOAT, 0);
  class_addmethod(nn_tilde_class, (t_method)nn_tilde_method, gensym("method"), A_SYMBOL, 0);
  class_addmethod(nn_tilde_class, (t_method)nn_tilde_available_models, gensym("print_available_models"), A_NULL);
  class_addmethod(nn_tilde_class, (t_method)nn_tilde_download, gensym("download"), A_GIMME, 0);
  class_addmethod(nn_tilde_class, (t_method)nn_tilde_remove, gensym("remove"), A_SYMBOL, 0);
  class_addbang(nn_tilde_class, (t_method)nn_tilde_bang);
  CLASS_MAINSIGNALIN(nn_tilde_class, t_nn_tilde, f);
}
