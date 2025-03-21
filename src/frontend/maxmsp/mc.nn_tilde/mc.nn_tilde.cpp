#include "../shared/nn_base.h"
#include "c74_min.h"

template<typename nn_class>
void check_loop_buffers(nn_class *mc_nn_instance, std::vector<float *> &in_model, std::vector<float *> &out_model) {
  if (mc_nn_instance->m_in_model.size() != in_model.size())
  {
    in_model.clear();
    for (auto &ptr : mc_nn_instance->m_in_model)
      in_model.push_back(ptr.get());

  }
  if (mc_nn_instance->m_out_model.size() != out_model.size())
  {
    out_model.clear();
    for (auto &ptr : mc_nn_instance->m_out_model)
      out_model.push_back(ptr.get());
  }
}

template <typename nn_class>
void model_perform(nn_class* nn_instance) {
  std::vector<float *> in_model, out_model;
  for (int c(0); c < nn_instance->n_inlets; c++)
    in_model.push_back(nn_instance->m_in_model[c].get());
  for (int c(0); c < nn_instance->n_outlets; c++)
    out_model.push_back(nn_instance->m_out_model[c].get());

  nn_instance->m_model->perform(in_model, out_model, nn_instance->m_buffer_size,
                                nn_instance->m_method, nn_instance->get_batches());
}

template <typename nn_class>
void model_perform_async(nn_class* nn_instance) {
  while (!nn_instance->m_ready){
    std::this_thread::sleep_for(std::chrono::milliseconds(REFRESH_THREAD_INTERVAL));
    if (nn_instance->m_should_stop_perform_thread) {
      return;
    }
  }
  std::vector<float *> in_model, out_model;
  for (int c(0); c < nn_instance->n_inlets; c++)
    in_model.push_back(nn_instance->m_in_model[c].get());
  for (int c(0); c < nn_instance->n_outlets; c++)
    out_model.push_back(nn_instance->m_out_model[c].get());

  while (!nn_instance->m_should_stop_perform_thread) {
    check_loop_buffers(nn_instance, in_model, out_model);
    if (nn_instance->m_data_available_lock.try_acquire_for(
            std::chrono::milliseconds(REFRESH_THREAD_INTERVAL))) {
        nn_instance->m_model->perform(in_model, out_model,
                                      nn_instance->m_buffer_size,
                                      nn_instance->m_method, 
                                      nn_instance->get_batches());
        nn_instance->m_result_available_lock.release();
    }
  }
}

long simplemc_multichanneloutputs(c74::max::t_object *x, long index,
                                  long count);
long simplemc_inputchanged(c74::max::t_object *x, long index, long count);


class mc_nn: public nn_base<mc_nn, mc_operator<>> {

public:
    MIN_DESCRIPTION{"Interface for deep learning models"};
    MIN_TAGS{"audio, deep learning, ai"};
    MIN_AUTHOR{"Antoine Caillon & Axel Chemla--Romeu-Santos"};

    static std::string get_external_name() { return "mc.nn~";} 
    mc_nn(const atoms &args = {}) {
        init_external(args); 
    }

    int get_sample_rate() override {
        return samplerate(); 
    }

    void init_external(const atoms &args) override {
        init_model(); 
        init_downloader(); 
        if (!args.size()) { return; } 
        init_inputs_and_outputs(args);
        init_inlets_and_outlets();
        init_buffers(); 
        init_process();
    } 
    void perform(audio_bundle input, audio_bundle output) override;

    // channel handling
    std::vector<int> chans;
    void reset_buffers(); 
    int get_batches(); 
    bool check_inputs(); 

    void init_inputs_and_outputs(const atoms& atoms) override; 
    void init_inlets_and_outlets() override; 
    void init_buffers() override; 
    void init_process() override; 

    message<> maxclass_setup{
      this, "maxclass_setup",
      [this](const c74::min::atoms &args, const int inlet) -> c74::min::atoms {
        cout << "mc.nn~ " << VERSION << " - torch " << TORCH_VERSION
             << " - 2024-2025 - Antoine Caillon & Axel Chemla--Romeu-Santos" << endl;
        cout << "visit https://www.github.com/acids-ircam" << endl;
        c74::max::t_class *c = args[0];
        c74::max::class_addmethod(
            c, (c74::max::method)simplemc_multichanneloutputs,
            "multichanneloutputs", c74::max::A_CANT, 0);
        c74::max::class_addmethod(c, (c74::max::method)simplemc_inputchanged,
                                  "inputchanged", c74::max::A_CANT, 0);
        return {};
      }};
};

bool mc_nn::check_inputs() {
  bool check = true;
  for (int i = 1; i < chans.size(); i++) {
    if (chans[i] != chans[0])
      check = false;
  }
  return check;
}

int mc_nn::get_batches() {
  return *std::min_element(chans.begin(), chans.end());
}

void mc_nn::init_inputs_and_outputs(const atoms& args)  {
    nn_base::init_inputs_and_outputs(args);
    for (int i(0); i < m_model_in; i++)
        chans.push_back(1);
}

void mc_nn::init_inlets_and_outlets() {
  for (int i(0); i < n_inlets; i++) {
    std::string input_label = "";
    try {
      input_label = m_model->get_model()
                        .attr(m_method + "_input_labels")
                        .toList()
                        .get(i)
                        .toStringRef();
    } catch (...) {
      input_label = "(multisignal) model input " + std::to_string(i);
    }
    m_inlets.push_back(std::make_unique<inlet<>>(this, input_label, "multichannelsignal"));
  }

  for (int i(0); i < n_outlets; i++) {
    std::string output_label = "";
    try {
      output_label = m_model->get_model()
                         .attr(m_method + "_output_labels")
                         .toList()
                         .get(i)
                         .toStringRef();
    } catch (...) {
      output_label = "(multisignal) model output " + std::to_string(i);
    }
    m_outlets.push_back(
        std::make_unique<outlet<>>(this, output_label, "multichannelsignal"));
  }
}

void mc_nn::init_buffers() {
  if (m_buffer_size == -1) {
    // NO THREAD MODE
    m_buffer_size = DEFAULT_BUFFER_SIZE;
  }
  if (m_buffer_size < m_higher_ratio) {
    cerr << "buffer size too small, switching to " << m_buffer_size << endl;
    m_buffer_size = m_higher_ratio;
  } else {
    m_buffer_size = power_ceil(m_buffer_size);
  }

    m_in_buffer = std::make_unique<circular_buffer<double, float>[]>(
      m_model_in * get_batches());
    for (int i(0); i < m_model_in * get_batches(); i++) {
        m_in_buffer[i].initialize(m_buffer_size);
        m_in_model.push_back(std::make_unique<float[]>(m_buffer_size));
    }

    m_out_buffer = std::make_unique<circular_buffer<float, double>[]>(
      m_model_out * get_batches());
    for (int i(0); i < m_model_out * get_batches(); i++) {
        m_out_buffer[i].initialize(m_buffer_size);
        m_out_model.push_back(std::make_unique<float[]>(m_buffer_size));
    }
}

void mc_nn::init_process() {
    nn_base<mc_nn, mc_operator<>>::init_process(); 
    if (m_use_thread) {
        m_compute_thread = std::make_unique<std::thread>(model_perform_async<mc_nn>, this);
    }
}

void mc_nn::perform(audio_bundle input, audio_bundle output) {
  auto vec_size = input.frame_count();

  // COPY INPUT TO CIRCULAR BUFFER
  int dim_offset = 0;
  for (int i(0); i < m_inlets.size(); i++) {
    for (int b(0); b < get_batches(); b++) {
      auto in = input.samples(dim_offset + b);
      m_in_buffer[i * get_batches() + b].put(in, vec_size);
    }
    dim_offset += chans[i];
  }

  if (m_in_buffer[0].full()) { // BUFFER IS FULL
    if (!m_use_thread) {
      // TRANSFER MEMORY BETWEEN INPUT CIRCULAR BUFFER AND MODEL BUFFER
      for (int c(0); c < n_inlets * get_batches(); c++)
        m_in_buffer[c].get(m_in_model[c].get(), m_buffer_size);

      // CALL MODEL PERFORM IN CURRENT THREAD
      model_perform(this);

      // TRANSFER MEMORY BETWEEN OUTPUT CIRCULAR BUFFER AND MODEL BUFFER
      for (int c(0); c < n_outlets * get_batches(); c++)
        m_out_buffer[c].put(m_out_model[c].get(), m_buffer_size);

    } else if (m_result_available_lock.try_acquire()) {
      // TRANSFER MEMORY BETWEEN INPUT CIRCULAR BUFFER AND MODEL BUFFER
      for (int c(0); c < n_inlets * get_batches(); c++)
        m_in_buffer[c].get(m_in_model[c].get(), m_buffer_size);

      // TRANSFER MEMORY BETWEEN OUTPUT CIRCULAR BUFFER AND MODEL BUFFER
      for (int c(0); c < n_outlets * get_batches(); c++)
        m_out_buffer[c].put(m_out_model[c].get(), m_buffer_size);

      // SIGNAL PERFORM THREAD THAT DATA IS AVAILABLE
      m_data_available_lock.release();
    }
  }

  // COPY CIRCULAR BUFFER TO OUTPUT
  for (int b(0); b < get_batches(); b++) {
    for (int d(0); d < m_outlets.size(); d++) {
      auto out = output.samples(d * get_batches() + b);
      m_out_buffer[b * m_outlets.size() + d].get(out, vec_size);
    }
  }
}

long simplemc_multichanneloutputs(c74::max::t_object *x, long index,
                                  long count) {
  minwrap<mc_nn> *ob = (minwrap<mc_nn> *)(x);
  return ob->m_min_object.get_batches();
}

long simplemc_inputchanged(c74::max::t_object *x, long index, long count) {
  minwrap<mc_nn> *ob = (minwrap<mc_nn> *)(x);
  bool needs_refresh = false;
  if (ob->m_min_object.chans[index] != count) {
    auto old_n_batch = ob->m_min_object.get_batches();
    ob->m_min_object.chans[index] = count;
    auto new_n_batch = ob->m_min_object.get_batches();
    if (old_n_batch != new_n_batch) {
      ob->m_min_object.init_buffers();
    }
    needs_refresh = true;
  }
  return needs_refresh;
}



MIN_EXTERNAL(mc_nn);