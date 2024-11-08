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
  int m_multichannel;  // Flag for multichannel mode

  int m_enabled;
  // BACKEND RELATED MEMBERS
  std::unique_ptr<Backend> m_model;
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

void model_perform(t_nn_tilde *nn_instance) {
  std::vector<float *> in_model, out_model;

  for (int c(0); c < nn_instance->m_in_dim; c++)
    in_model.push_back(nn_instance->m_in_model[c].get());
  for (int c(0); c < nn_instance->m_out_dim; c++)
    out_model.push_back(nn_instance->m_out_model[c].get());

  nn_instance->m_model->perform(in_model, out_model, nn_instance->m_buffer_size,
                                nn_instance->m_method->s_name, 1);
}

// DSP CALL
t_int *nn_tilde_perform(t_int *w) {
  t_nn_tilde *x = (t_nn_tilde *)(w[1]);

  if (!x->m_model->is_loaded() || !x->m_enabled) {
    for (int c(0); c < x->m_out_dim; c++) {
      for (int i(0); i < x->m_dsp_vec_size; i++) {
        x->m_dsp_out_vec[c][i] = 0;
      }
    }
    return w + 2;
  }

  // COPY INPUT TO CIRCULAR BUFFER
  for (int c(0); c < x->m_in_dim; c++) {
    x->m_in_buffer[c].put(x->m_dsp_in_vec[c], x->m_dsp_vec_size);
  }

  if (x->m_in_buffer[0].full()) { // BUFFER IS FULL
    // IF USE THREAD, CHECK THAT COMPUTATION IS OVER
    if (x->m_compute_thread && x->m_use_thread) {
      x->m_compute_thread->join();
    }

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
  for (int c(0); c < x->m_out_dim; c++) {
    x->m_out_buffer[c].get(x->m_dsp_out_vec[c], x->m_dsp_vec_size);
  }

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
    for (int i = 0; i < x->m_in_dim; i++) {
      x->m_dsp_in_vec.push_back(sp[0]->s_vec + x->m_dsp_vec_size * (i % nchans));
    }

    // Configure multichannel output
    g_signal_setmultiout(&sp[1], x->m_out_dim);
    for (int i = 0; i < x->m_out_dim; i++) {
      x->m_dsp_out_vec.push_back(sp[1]->s_vec + x->m_dsp_vec_size * i);
    }
  } else {
    // Standard mode - separate signals
    for (int i = 0; i < x->m_in_dim; i++) {
      x->m_dsp_in_vec.push_back(sp[i]->s_vec);
    }
    for (int i(x->m_in_dim); i < x->m_in_dim + x->m_out_dim; i++) {
      if (g_signal_setmultiout) {
        g_signal_setmultiout(&sp[i], 1); // Ensure single channel
      }
      x->m_dsp_out_vec.push_back(sp[i]->s_vec);
    }
  }
  dsp_add(nn_tilde_perform, 1, x);
}

void nn_tilde_free(t_nn_tilde *x) {
  if (x->m_compute_thread) {
    x->m_compute_thread->join();
  }
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
        if (fd >= 0) {
            return fd;
        }

        // If no extension was given, try with .ts
        if (!strrchr(fname, '.')) {
            std::string with_ext = std::string(fname) + ".ts";
            fd = canvas_open(canvas, with_ext.c_str(), "", path, name, MAXPDSTRING, 1);
            if (fd >= 0) {
                return fd;
            }
        }
        return -1;
    };

    // if it's already an absolute path, normalize and return it
    if (sys_isabsolutepath(filename)) {
        return normalize_path(filename);
    }

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
  x->m_multichannel = 0;  // Default to non-multichannel mode

  // CHECK ARGUMENTS
  // Check for -m flag as first argument
  if (argc > 0 && argv->a_type == A_SYMBOL && atom_getsymbol(argv) == gensym("-m")) {
    if (g_signal_setmultiout) {
      x->m_multichannel = 1;
    } else {
      int maj = 0, min = 0, bug = 0;
      sys_getversion(&maj, &min, &bug);
      pd_error(x, "nn~: no multichannel support in Pd %i.%i-%i, ignoring '-m' flag", 
              maj, min, bug);
    }
    argc--;
    argv++;
  }

  if (!argc) {
    return (void *)x;
  }
  if (argc > 0) {
    x->m_path = atom_gensym(argv);
  }
  if (argc > 1) {
    x->m_method = atom_gensym(argv + 1);
  }
  if (argc > 2) {
    x->m_buffer_size = atom_getint(argv + 2);
  }

  // SEARCH FOR FILE
  std::string fullpath = resolve_file_path((t_object *)x, x->m_path->s_name);
  if (!fullpath.empty()) {
      x->m_path = gensym(fullpath.c_str());
      // proceed with using the file...
  }

  // TRY TO LOAD MODEL
  if (x->m_model->load(x->m_path->s_name)) {
    pd_error(x, "error during loading");
    return (void *)x;
  } else {
    // cout << "successfully loaded model" << endl;
  }

  // GET MODEL'S METHOD PARAMETERS
  auto params = x->m_model->get_method_params(x->m_method->s_name);
  x->settable_attributes = x->m_model->get_settable_attributes();

  if (!params.size()) {
    post("method not found, using forward instead");
    x->m_method = gensym("forward");
    params = x->m_model->get_method_params(x->m_method->s_name);
  }

  x->m_in_dim = params[0];
  x->m_in_ratio = params[1];
  x->m_out_dim = params[2];
  x->m_out_ratio = params[3];

  auto higher_ratio = x->m_model->get_higher_ratio();

  if (!x->m_buffer_size) {
    // NO THREAD MODE
    x->m_use_thread = false;
    x->m_buffer_size = higher_ratio;
  } else if (x->m_buffer_size < higher_ratio) {
    x->m_buffer_size = higher_ratio;
    std::string err_message = "buffer size too small, switching to ";
    err_message += std::to_string(higher_ratio);
    post(err_message.c_str());
  } else {
    x->m_buffer_size = power_ceil(x->m_buffer_size);
  }

  // CREATE INLETS, OUTLETS and BUFFERS
  x->m_in_buffer =
      std::make_unique<circular_buffer<float, float>[]>(x->m_in_dim);
  for (int i(0); i < x->m_in_dim; i++) {
    x->m_in_buffer[i].initialize(x->m_buffer_size);
    x->m_in_model.push_back(std::make_unique<float[]>(x->m_buffer_size));
  }

  x->m_out_buffer =
      std::make_unique<circular_buffer<float, float>[]>(x->m_out_dim);
  for (int i(0); i < x->m_out_dim; i++) {
    x->m_out_buffer[i].initialize(x->m_buffer_size);
    x->m_out_model.push_back(std::make_unique<float[]>(x->m_buffer_size));
  }

  // Create inlets and outlets based on multichannel mode
  if (!x->m_multichannel) {
    // Standard mode: create individual inlets/outlets
    for (int i(0); i < x->m_in_dim - 1; i++)
      inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);
    for (int i(0); i < x->m_out_dim; i++)
      outlet_new(&x->x_obj, &s_signal);
  } else {
    // Multichannel mode: only create a single outlet
    // Note: The first inlet is created automatically by CLASS_MAINSIGNALIN
    outlet_new(&x->x_obj, &s_signal);
  }
  return (void *)x;
}

void nn_tilde_enable(t_nn_tilde *x, t_floatarg arg) { x->m_enabled = int(arg); }
void nn_tilde_reload(t_nn_tilde *x) { x->m_model->reload(); }

void nn_tilde_set(t_nn_tilde *x, t_symbol *s, int argc, t_atom *argv) {
  if (argc < 2) {
    pd_error(x, "set needs at least 2 arguments [set argname argval1 ...)");
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
  class_addmethod(nn_tilde_class, (t_method)nn_tilde_reload, gensym("reload"),
                  A_NULL);
  class_addmethod(nn_tilde_class, (t_method)nn_tilde_set, gensym("set"),
                  A_GIMME, A_NULL);
  CLASS_MAINSIGNALIN(nn_tilde_class, t_nn_tilde, f);
}
