#include "../../../backend/backend.h"
#include "../../maxmsp/shared/circular_buffer.h"
#include "m_pd.h"
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

using t_signal_setmultiout = void (*)(t_signal **, int);
static t_signal_setmultiout g_signal_setmultiout;

static t_class *nn_tilde_class;

#ifndef VERSION
#define VERSION "UNDEFINED"
#endif

#if PD_MINOR_VERSION >= 54
# define PD_HAVE_MULTICHANNEL
#else
# pragma message("building without multi-channel support; requires Pd 0.54+")
# define CLASS_MULTICHANNEL 0
#endif

unsigned power_ceil(unsigned x) {
  if (x <= 1)
    return 1;
  int power = 2;
  x--;
  while (x >>= 1)
    power <<= 1;
  return power;
}

// CLASS LIKE INITIALISATION
typedef struct _nn_tilde {
  t_object x_obj;
  t_sample f;
  int m_multichannel;
  bool m_outchannels_changed;
  t_outlet* m_info_outlet;

  int m_enabled;
  // BACKEND RELATED MEMBERS
  std::unique_ptr<Backend> m_model;
  std::vector<float*> m_in_model_ptrs;
  std::vector<float*> m_out_model_ptrs;
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
} t_nn_tilde;

void model_perform(t_nn_tilde *x) {
  // Use pre-allocated vectors
  x->m_model->perform(x->m_in_model_ptrs, x->m_out_model_ptrs, 
                     x->m_buffer_size, x->m_method->s_name, 1);
}

// DSP CALL
t_int *nn_tilde_perform(t_int *w) {
  t_nn_tilde *x = (t_nn_tilde *)(w[1]);

  if (!x->m_model->is_loaded() || !x->m_enabled) {
    for (int c(0); c < x->m_out_dim; c++) {
      float *out = x->m_dsp_out_vec[c];
      memset(out, 0, x->m_dsp_vec_size * sizeof(float));
    }
    return w + 2;
  }

  // COPY INPUT TO CIRCULAR BUFFER
  for (int c(0); c < x->m_in_dim; c++)
    x->m_in_buffer[c].put(x->m_dsp_in_vec[c], x->m_dsp_vec_size);

  if (x->m_in_buffer[0].full()) { // BUFFER IS FULL
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

void nn_tilde_dsp(t_nn_tilde *x, t_signal **sp) {
  x->m_dsp_vec_size = sp[0]->s_n;
  x->m_dsp_in_vec.clear();
  x->m_dsp_out_vec.clear();

  if (x->m_multichannel) {
    // Get number of available input channels
    int nchans = sp[0]->s_nchans;

    // Map input channels, wrapping if needed
    for (int i(0); i < x->m_in_dim; i++)
      x->m_dsp_in_vec.push_back(sp[0]->s_vec + x->m_dsp_vec_size * (i % nchans));

    // Configure multichannel output
    g_signal_setmultiout(&sp[1], x->m_out_dim);
    for (int i(0); i < x->m_out_dim; i++)
      x->m_dsp_out_vec.push_back(sp[1]->s_vec + x->m_dsp_vec_size * i);
  } else {
    // Standard mode - separate signals
    for (int i(0); i < x->m_in_dim; i++)
      x->m_dsp_in_vec.push_back(sp[i]->s_vec);
    for (int i(x->m_in_dim); i < x->m_in_dim + x->m_out_dim; i++) {
      if (g_signal_setmultiout)
        g_signal_setmultiout(&sp[i], 1); // Ensure single channel
      x->m_dsp_out_vec.push_back(sp[i]->s_vec);
    }
  }
  dsp_add(nn_tilde_perform, 1, x);
}

void nn_tilde_free(t_nn_tilde *x) {
  if (x->m_compute_thread) x->m_compute_thread->join();

  for (int i(1); i < x->m_in_dim; i++)
    inlet_free(x->x_obj.ob_inlet);

  if (x->m_multichannel) outlet_free(x->m_info_outlet);
  for (int i(0); i < x->m_out_dim; i++)
    outlet_free(x->x_obj.ob_outlet);
}

std::string resolve_file_path(t_object *obj, const char *filename) {
  // Normalize a path - resolve .. and . components, standardize separators
  auto normalize_path = [](const std::string& path) {
    char buf[MAXPDSTRING];
    char *bufptr = buf;
    strncpy(buf, path.c_str(), MAXPDSTRING);
    // Convert backslashes to forward slashes on Windows
    for (char *p = buf; *p; p++) {
      if (*p == '\\') *p = '/';
    }
    // Remove any "//" 
    char *p1 = buf;
    char *p2 = buf;
    while (*p2) {
      if (*p2 == '/' && *(p2+1) == '/') {
        p2++;
      } else {
        *p1++ = *p2++;
      }
    }
    *p1 = '\0';
    return std::string(buf);
  };

  // Helper to try opening with both original name and .ts extension
  auto try_open = [](t_canvas *canvas, const char *fname, char *path, char **name) {
    // Try with original filename
    int fd = canvas_open(canvas, fname, "", path, name, MAXPDSTRING, 1);
    if (fd >= 0) return fd;

    // If no extension was given, try with .ts
    if (!strrchr(fname, '.')) {
      std::string with_ext = std::string(fname) + ".ts";
      fd = canvas_open(canvas, with_ext.c_str(), "", path, name, MAXPDSTRING, 1);
      if (fd >= 0) return fd;
    }
    return -1;
  };

  // if it's already an absolute path, normalize and return it
  if (sys_isabsolutepath(filename)) return normalize_path(filename);

  // try canvas-relative path first
  char path[MAXPDSTRING];
  char *name = nullptr;
  t_canvas *canvas = canvas_getcurrent();
  int fd = try_open(canvas, filename, path, &name);
  if (fd >= 0) {
    sys_close(fd);
    char fullpath[MAXPDSTRING];
    snprintf(fullpath, MAXPDSTRING, "%s/%s", path, name);
    return normalize_path(fullpath);
  }

  // then try Pd's search paths
  char dirresult[MAXPDSTRING];
  fd = open_via_path("", filename, ".ts", dirresult, &name, MAXPDSTRING, 1);
  if (fd >= 0) {
    sys_close(fd);
    char fullpath[MAXPDSTRING];
    snprintf(fullpath, MAXPDSTRING, "%s/%s", dirresult, name);
    return normalize_path(fullpath);
  }

  // file not found - return empty string to indicate failure
  pd_error(obj, "could not find file '%s' (or %s.ts)", filename, filename);
  return "";
}

void create_buffers(t_nn_tilde *x, int in_dim, int out_dim) {
  x->m_in_buffer = std::make_unique<circular_buffer<float, float>[]>(in_dim);
  x->m_out_buffer = std::make_unique<circular_buffer<float, float>[]>(out_dim);
  
  x->m_in_model.clear();
  x->m_out_model.clear();
  
  for (int i(0); i < in_dim; i++) {
    x->m_in_buffer[i].initialize(x->m_buffer_size);
    x->m_in_model.push_back(std::make_unique<float[]>(x->m_buffer_size));
  }
  
  for (int i(0); i < out_dim; i++) {
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

// MODEL LOADER
bool nn_tilde_load_model(t_nn_tilde *x, const char *path) {
  // Store old dimensions to check if they changed
  int old_in_dim = x->m_in_dim;
  int old_out_dim = x->m_out_dim;

  // Resolve the file path
  std::string fullpath = resolve_file_path((t_object *)x, path);
  if (fullpath.empty()) return false;

  // Create a new backend instance
  auto new_model = std::make_unique<Backend>();
  if (new_model->load(fullpath.c_str())) {
    pd_error(x, "error loading model %s", path);
    return false;
  }

  // Get the method parameters
  auto params = new_model->get_method_params(x->m_method->s_name);
  if (!params.size()) {
    post("method %s not found in model, using forward instead", x->m_method->s_name);
    x->m_method = gensym("forward");
    params = new_model->get_method_params("forward");
    if (!params.size()) {
      pd_error(x, "forward method not found in model");
      return false;
    }
  }

  // Store dimensions and ratios
  int new_in_dim = params[0];
  int new_in_ratio = params[1];
  int new_out_dim = params[2];
  int new_out_ratio = params[3];

  // Check if dimensions changed - this affects whether we need DSP update
  bool dims_changed = (new_in_dim != old_in_dim) || (new_out_dim != old_out_dim);

  // Check/adjust buffer size
  auto higher_ratio = new_model->get_higher_ratio();
  if (!x->m_buffer_size) {
    // NO THREAD MODE
    x->m_use_thread = false;
    x->m_buffer_size = higher_ratio;
  } else if (x->m_buffer_size < higher_ratio) {
    x->m_buffer_size = higher_ratio;
    post("buffer size adjusted to %d", x->m_buffer_size);
  } else {
    x->m_buffer_size = power_ceil(x->m_buffer_size);
  }

  // Only create new buffers if dimensions changed
  if (dims_changed) {
    create_buffers(x, new_in_dim, new_out_dim);
    x->m_outchannels_changed = true;
  }

  // Update model and parameters
  x->m_model = std::move(new_model);
  x->m_path = gensym(fullpath.c_str());
  x->m_in_dim = new_in_dim;
  x->m_in_ratio = new_in_ratio;
  x->m_out_dim = new_out_dim;
  x->m_out_ratio = new_out_ratio;
  x->settable_attributes = x->m_model->get_settable_attributes();

  // Output loaded message if in multichannel mode
  if (x->m_multichannel) {
    t_atom path_atom;
    SETSYMBOL(&path_atom, x->m_path);
    outlet_anything(x->m_info_outlet, gensym("loaded"), 1, &path_atom);
  }

  return true;
}

void nn_tilde_bang(t_nn_tilde *x) {
  if (!x->m_multichannel) {
    pd_error(x, "nn~: info output only available in multichannel mode");
    return;
  }

  // Output "is_loaded" status
  t_atom is_loaded;
  SETFLOAT(&is_loaded, x->m_model->is_loaded());
  outlet_anything(x->m_info_outlet, gensym("is_loaded"), 1, &is_loaded);

  // Return if no model is loaded
  if (!x->m_model->is_loaded()) return;

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

  // Output buffer size
  t_atom bufsize;
  SETFLOAT(&bufsize, x->m_buffer_size);
  outlet_anything(x->m_info_outlet, gensym("bufsize"), 1, &bufsize);

  // Output "enabled" status
  t_atom enabled;
  SETFLOAT(&enabled, x->m_enabled);
  outlet_anything(x->m_info_outlet, gensym("enabled"), 1, &enabled);

  t_atom model;
  SETSYMBOL(&model, x->m_path);
  outlet_anything(x->m_info_outlet, gensym("model"), 1, &model);

  // Output dimensions
  std::vector<std::string> methods = x->m_model->get_available_methods();
  std::vector<t_atom> method_atoms(methods.size());    
  for (size_t i = 0; i < methods.size(); i++)
    SETSYMBOL(&method_atoms[i], gensym(methods[i].c_str()));

  outlet_anything(x->m_info_outlet, gensym("methods"), 
                 methods.size(), method_atoms.data());

  // Output settable attributes
  std::vector<t_atom> attr_atoms(x->settable_attributes.size());
  for (size_t i = 0; i < x->settable_attributes.size(); i++)
      SETSYMBOL(&attr_atoms[i], gensym(x->settable_attributes[i].c_str()));
  
  outlet_anything(x->m_info_outlet, gensym("attributes"), 
                 attr_atoms.size(), attr_atoms.data());
}

void *nn_tilde_new(t_symbol *s, int argc, t_atom *argv) {
  t_nn_tilde *x = (t_nn_tilde *)pd_new(nn_tilde_class);

  x->m_model = std::make_unique<Backend>();
  x->m_head = 0;
  x->m_compute_thread = nullptr;
  x->m_in_dim = 1;
  x->m_in_ratio = 1;
  x->m_out_dim = 1;
  x->m_out_ratio = 1;
  x->m_buffer_size = 4096;
  x->m_method = gensym("forward");
  x->m_enabled = 1;
  x->m_use_thread = true;
  x->m_multichannel = 0;
  x->m_outchannels_changed = true;

  // Create minimum outlet (we already have main inlet from CLASS_MAINSIGNALIN)
  outlet_new(&x->x_obj, &s_signal);
  create_buffers(x, 1, 1);  // minimum buffers for one in/out

  // CHECK ARGUMENTS
  // Check for -m flag as first argument
  if (argc > 0 && argv->a_type == A_SYMBOL && atom_getsymbol(argv) == gensym("-m")) {
    if (g_signal_setmultiout) {
      x->m_multichannel = 1;
      // Add info outlet in multichannel mode
      x->m_info_outlet = outlet_new(&x->x_obj, &s_anything);
    } else {
      int maj = 0, min = 0, bug = 0;
      sys_getversion(&maj, &min, &bug);
      pd_error(x, "nn~: no multichannel support in Pd %i.%i-%i, ignoring '-m' flag", 
              maj, min, bug);
    }
    argc--;
    argv++;
  }

  if (!argc) return (void *)x;

  x->m_path = atom_getsymbol(argv);
  if (argc > 1) x->m_method = atom_gensym(argv + 1);
  if (argc > 2) x->m_buffer_size = atom_getint(argv + 2);

  if (nn_tilde_load_model(x, x->m_path->s_name)) {
    // Add any additional inlets/outlets needed for model
    if (!x->m_multichannel) {
      for (int i(1); i < x->m_in_dim; i++)
        inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);
      for (int i(1); i < x->m_out_dim; i++)
        outlet_new(&x->x_obj, &s_signal);
    }
  }

  return (void *)x;
}

void nn_tilde_load(t_nn_tilde *x, t_symbol *s) {
  if (!x->m_multichannel) {
    pd_error(x, "nn~: dynamically loading models is only supported in multichannel mode");
    return;
  }

  // Wait for any ongoing computation
  if (x->m_compute_thread) {
    x->m_compute_thread->join();
    x->m_compute_thread = nullptr;
  }

  // Load the new model
  if (nn_tilde_load_model(x, s->s_name) && x->m_outchannels_changed)
      canvas_update_dsp();
}

void nn_tilde_enable(t_nn_tilde *x, t_floatarg arg) { x->m_enabled = int(arg); }
void nn_tilde_reload(t_nn_tilde *x) { x->m_model->reload(); }

void nn_tilde_set(t_nn_tilde *x, t_symbol *s, int argc, t_atom *argv) {
  if (argc < 2) {
    pd_error(x, "set needs at least 2 arguments [set argname argval1 ...(");
    return;
  }
  std::vector<std::string> attribute_args;

  auto argname = argv[0].a_w.w_symbol->s_name;
  std::string argname_str = argname;

  if (!std::count(x->settable_attributes.begin(), x->settable_attributes.end(),
                  argname_str)) {
    pd_error(x, "argument name not settable in current model");
    return;
  }

  for (int i(1); i < argc; i++) {
    if (argv[i].a_type == A_SYMBOL) {
      attribute_args.push_back(argv[i].a_w.w_symbol->s_name);
    } else if (argv[i].a_type == A_FLOAT) {
      attribute_args.push_back(std::to_string(argv[i].a_w.w_float));
    }
  }
  try {
    x->m_model->set_attribute(argname, attribute_args);
  } catch (const std::exception &e) {
    pd_error(x, "nn~: %s", e.what());
  }
}

void startup_message() {
  std::string startmessage = "nn~ - ";
  startmessage += VERSION;
  startmessage += " - ";
  startmessage += "torch ";
  startmessage += TORCH_VERSION;
  startmessage += " - 2024 - Antoine Caillon et al.";
  post(startmessage.c_str());
}

#ifdef _WIN32
#define EXPORT extern "C" __declspec(dllexport)
#elif __GNUC__ >= 4
#define EXPORT extern "C" __attribute__((visibility("default")))
#else
#define EXPORT extern "C"
#endif

EXPORT void nn_tilde_setup(void) {
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
      (LPCSTR)&pd_typedmess, &module)) {
    g_signal_setmultiout = (t_signal_setmultiout)(void *)GetProcAddress(
      module, "signal_setmultiout");
  }
#else
  // search recursively, starting from the main program
  g_signal_setmultiout = (t_signal_setmultiout)dlsym(
    dlopen(nullptr, RTLD_NOW), "signal_setmultiout");
#endif
#endif // PD_HAVE_MULTICHANNEL
  startup_message();
  nn_tilde_class = class_new(gensym("nn~"), (t_newmethod)nn_tilde_new, 0,
                             sizeof(t_nn_tilde), CLASS_MULTICHANNEL, A_GIMME, 0);
  class_addmethod(nn_tilde_class, (t_method)nn_tilde_dsp, gensym("dsp"), A_CANT,
                  0);
  class_addmethod(nn_tilde_class, (t_method)nn_tilde_enable, gensym("enable"),
                  A_DEFFLOAT, A_NULL);
  class_addmethod(nn_tilde_class, (t_method)nn_tilde_load, gensym("load"),
                  A_SYMBOL, 0);
  class_addmethod(nn_tilde_class, (t_method)nn_tilde_reload, gensym("reload"),
                  A_NULL);
  class_addmethod(nn_tilde_class, (t_method)nn_tilde_set, gensym("set"),
                  A_GIMME, A_NULL);
  class_addbang(nn_tilde_class, (t_method)nn_tilde_bang);
  CLASS_MAINSIGNALIN(nn_tilde_class, t_nn_tilde, f);
}
