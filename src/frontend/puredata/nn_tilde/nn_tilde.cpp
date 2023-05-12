#include "../../../backend/backend.h"
#include "../../maxmsp/shared/circular_buffer.h"
#include "m_pd.h"
#include <memory>
#include <string>
#include <vector>

static t_class *nn_tilde_class;

#ifndef VERSION
#define VERSION "UNDEFINED"
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

  int m_enabled;
  // BACKEND RELATED MEMBERS
  Backend m_model;
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

  nn_instance->m_model.perform(in_model, out_model, nn_instance->m_buffer_size,
                               nn_instance->m_method->s_name, 1);
}

// DSP CALL
t_int *nn_tilde_perform(t_int *w) {
  t_nn_tilde *x = (t_nn_tilde *)(w[1]);

  if (!x->m_model.is_loaded() || !x->m_enabled) {
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

  for (int i(0); i < x->m_in_dim; i++) {
    x->m_dsp_in_vec.push_back(sp[i]->s_vec);
  }
  for (int i(x->m_in_dim); i < x->m_in_dim + x->m_out_dim; i++) {
    x->m_dsp_out_vec.push_back(sp[i]->s_vec);
  }
  dsp_add(nn_tilde_perform, 1, x);
}

void nn_tilde_free(t_nn_tilde *x) {
  if (x->m_compute_thread) {
    x->m_compute_thread->join();
  }
}

void *nn_tilde_new(t_symbol *s, int argc, t_atom *argv) {
  t_nn_tilde *x = (t_nn_tilde *)pd_new(nn_tilde_class);

  x->m_model = Backend();
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

  // CHECK ARGUMENTS
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
  if (!sys_isabsolutepath(x->m_path->s_name)) {
    char dirname[MAXPDSTRING], *dummy;
    auto fd = open_via_path("", x->m_path->s_name, "", dirname, &dummy,
                            MAXPDSTRING, 1);
    std::string found_path;
    found_path += dirname;
    found_path += "/";
    found_path += x->m_path->s_name;
    x->m_path = gensym(found_path.c_str());
  }

  // TRY TO LOAD MODEL
  if (x->m_model.load(x->m_path->s_name)) {
    post("error during loading");
    return (void *)x;
  } else {
    // cout << "successfully loaded model" << endl;
  }

  // GET MODEL'S METHOD PARAMETERS
  auto params = x->m_model.get_method_params(x->m_method->s_name);
  x->settable_attributes = x->m_model.get_settable_attributes();

  if (!params.size()) {
    post("method not found, using forward instead");
    x->m_method = gensym("forward");
    params = x->m_model.get_method_params(x->m_method->s_name);
  }

  x->m_in_dim = params[0];
  x->m_in_ratio = params[1];
  x->m_out_dim = params[2];
  x->m_out_ratio = params[3];

  auto higher_ratio = x->m_model.get_higher_ratio();

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
    if (i < x->m_in_dim - 1)
      inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);
    x->m_in_buffer[i].initialize(x->m_buffer_size);
    x->m_in_model.push_back(std::make_unique<float[]>(x->m_buffer_size));
  }

  x->m_out_buffer =
      std::make_unique<circular_buffer<float, float>[]>(x->m_out_dim);
  for (int i(0); i < x->m_out_dim; i++) {
    outlet_new(&x->x_obj, &s_signal);
    x->m_out_buffer[i].initialize(x->m_buffer_size);
    x->m_out_model.push_back(std::make_unique<float[]>(x->m_buffer_size));
  }

  return (void *)x;
}

void nn_tilde_enable(t_nn_tilde *x, t_floatarg arg) { x->m_enabled = int(arg); }

void nn_tilde_set(t_nn_tilde *x, t_symbol *s, int argc, t_atom *argv) {
  if (argc < 2) {
    post("set needs at least 2 arguments [set argname argval1 ...)");
    return;
  }
  std::vector<std::string> attribute_args;

  auto argname = argv[0].a_w.w_symbol->s_name;
  std::string argname_str = argname;

  if (!std::count(x->settable_attributes.begin(), x->settable_attributes.end(),
                  argname_str)) {
    post("argument name not settable in current model");
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
    x->m_model.set_attribute(argname, attribute_args);
  } catch (const std::exception &e) {
    post(e.what());
  }
}

void startup_message() {
  std::string startmessage = "nn~ - ";
  startmessage += VERSION;
  startmessage += " - ";
  startmessage += "torch ";
  startmessage += TORCH_VERSION;
  startmessage += " - 2023 - Antoine Caillon";
  post(startmessage.c_str());
}

extern "C" {
void nn_tilde_setup(void) {
  startup_message();
  nn_tilde_class = class_new(gensym("nn~"), (t_newmethod)nn_tilde_new, 0,
                             sizeof(t_nn_tilde), CLASS_DEFAULT, A_GIMME, 0);

  class_addmethod(nn_tilde_class, (t_method)nn_tilde_dsp, gensym("dsp"), A_CANT,
                  0);
  class_addmethod(nn_tilde_class, (t_method)nn_tilde_enable, gensym("enable"),
                  A_DEFFLOAT, A_NULL);
  class_addmethod(nn_tilde_class, (t_method)nn_tilde_set, gensym("set"),
                  A_GIMME, A_NULL);
  CLASS_MAINSIGNALIN(nn_tilde_class, t_nn_tilde, f);
}
}