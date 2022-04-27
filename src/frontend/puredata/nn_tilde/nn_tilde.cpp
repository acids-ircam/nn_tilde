#include "../../../backend/backend.h"
#include "m_pd.h"
#include "pthread.h"
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
  t_symbol *m_method, *m_path;
  std::unique_ptr<std::thread> compute_thread;

  // BUFFER RELATED MEMBERS
  int m_head, m_in_dim, m_in_ratio, m_out_dim, m_out_ratio, m_buffer_size;
  std::vector<std::unique_ptr<float[]>> m_in_buffer, m_out_buffer;
  bool m_use_thread;

  // DSP RELATED MEMBERS
  int m_dsp_vec_size;
  std::vector<float *> m_dsp_in_vec;
  std::vector<float *> m_dsp_out_vec;

} t_nn_tilde;

// THREAD PERFORM
void thread_perform(t_nn_tilde *nn_instance, std::vector<float *> in_buffer,
                    std::vector<float *> out_buffer, int n_vec,
                    std::string method) {
  // SET THREAD TO REALTIME PRIORITY
  pthread_t this_thread = pthread_self();
  struct sched_param params;
  params.sched_priority = sched_get_priority_max(SCHED_FIFO);
  int ret = pthread_setschedparam(this_thread, SCHED_FIFO, &params);
  nn_instance->m_model.perform(in_buffer, out_buffer, n_vec, method);
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

  for (int c(0); c < x->m_in_dim; c++) {
    for (int i(0); i < x->m_dsp_vec_size; i++) {
      x->m_in_buffer[c][i + x->m_head] = x->m_dsp_in_vec[c][i];
    }
  }

  for (int c(0); c < x->m_out_dim; c++) {
    for (int i(0); i < x->m_dsp_vec_size; i++) {
      x->m_dsp_out_vec[c][i] = x->m_out_buffer[c][i + x->m_head];
    }
  }

  // INCREASE HEAD
  x->m_head += x->m_dsp_vec_size;

  // IF BUFFER FILLED
  if (!(x->m_head % x->m_buffer_size)) {
    if (x->compute_thread) {
      x->compute_thread->join();
    }

    x->m_head = x->m_head % (2 * x->m_buffer_size);

    int offset_head = (x->m_head + x->m_buffer_size) % (2 * x->m_buffer_size);

    std::vector<float *> in_buffer;
    std::vector<float *> out_buffer;

    for (int i(0); i < x->m_in_buffer.size(); i++) {
      in_buffer.push_back(&x->m_in_buffer[i][offset_head]);
    }
    for (int i(0); i < x->m_out_buffer.size(); i++) {
      out_buffer.push_back(&x->m_out_buffer[i][offset_head]);
    }

    x->compute_thread =
        std::make_unique<std::thread>(thread_perform, x, in_buffer, out_buffer,
                                      x->m_buffer_size, x->m_method->s_name);
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
  if (x->compute_thread) {
    x->compute_thread->join();
  }
}

void *nn_tilde_new(t_symbol *s, int argc, t_atom *argv) {
  t_nn_tilde *x = (t_nn_tilde *)pd_new(nn_tilde_class);

  x->m_head = 0;
  x->compute_thread = nullptr;
  x->m_in_dim = 1;
  x->m_in_ratio = 1;
  x->m_out_dim = 1;
  x->m_out_ratio = 1;
  x->m_buffer_size = 4096;
  x->m_method = gensym("forward");
  x->m_enabled = 1;

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

  // TRY TO LOAD MODEL
  if (x->m_model.load(x->m_path->s_name)) {
    post("error during loading");
    return (void *)x;
  } else {
    // cout << "successfully loaded model" << endl;
  }

  // GET MODEL'S METHOD PARAMETERS
  auto params = x->m_model.get_method_params(x->m_method->s_name);

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
  x->m_in_buffer.push_back(std::make_unique<float[]>(2 * x->m_buffer_size));
  for (int i(0); i < x->m_in_dim - 1; i++) {
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);
    x->m_in_buffer.push_back(std::make_unique<float[]>(2 * x->m_buffer_size));
  }
  for (int i(0); i < x->m_out_dim; i++) {
    outlet_new(&x->x_obj, &s_signal);
    x->m_out_buffer.push_back(std::make_unique<float[]>(2 * x->m_buffer_size));
  }

  return (void *)x;
}

void nn_tilde_enable(t_nn_tilde *x, t_floatarg arg) { x->m_enabled = int(arg); }

void startup_message() {
  std::string startmessage = "nn~ - ";
  startmessage += VERSION;
  startmessage += " - 2022 - Antoine Caillon";
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
  CLASS_MAINSIGNALIN(nn_tilde_class, t_nn_tilde, f);
}
}