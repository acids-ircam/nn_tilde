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
  for (int c(0); c < nn_instance->m_model_in * nn_instance->get_batches(); c++)
    in_model.push_back(nn_instance->m_in_model[c].get());
  for (int c(0); c < nn_instance->m_model_out * nn_instance->get_batches(); c++)
    out_model.push_back(nn_instance->m_out_model[c].get());

  while (!nn_instance->m_should_stop_perform_thread) {
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


class mcs_nn: public nn_base<mcs_nn, mc_operator<>> {

public:
    MIN_DESCRIPTION{"Interface for deep learning models"};
    MIN_TAGS{"audio, deep learning, ai"};
    MIN_AUTHOR{"Antoine Caillon & Axel Chemla--Romeu-Santos"};

    static std::string get_external_name() { return "mcs.nn~";} 
    mcs_nn(const atoms &args = {}) {
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
    int get_batches(); 
    bool check_inputs(); 

    void init_inputs_and_outputs(const atoms& atoms) override; 
    void init_inlets_and_outlets() override; 
    void init_buffers() override; 
    void init_process() override; 

    message<> maxclass_setup{
      this, "maxclass_setup",
      [this](const c74::min::atoms &args, const int inlet) -> c74::min::atoms {
        cout << "mcs.nn~ " << VERSION << " - torch " << TORCH_VERSION
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

bool mcs_nn::check_inputs() {
    return true;
}

int mcs_nn::get_batches() {
  return n_batches;
}

void mcs_nn::init_inputs_and_outputs(const atoms &args) {
  bool empty_mode; 
  if (args.size() > 0) { // ONE ARGUMENT IS GIVEN
    auto model_path = std::string(args[0]);
    if (model_path == "void") {
      empty_mode = true;    
    } else {
      m_path = to_model_path(model_path);
    }
  }
  
  if (empty_mode) {
    if (args.size() > 1) { // FOUR ARGUMENTS ARE GIVEN
      auto n_batches = int(args[1]);
    }
    if (args.size() > 2) { // THREE ARGUMENTS ARE GIVEN
      m_buffer_size = int(args[2]);
    }
  } else {
    if (args.size() > 1) { // TWO ARGUMENTS ARE GIVEN
      m_method = std::string(args[1]);
    }
    if (args.size() > 2) { // TWO ARGUMENTS ARE GIVEN
      n_batches = int(args[2]);
    }
    if (args.size() > 3) { // THREE ARGUMENTS ARE GIVEN
      m_buffer_size = int(args[3]);
    }
    
    load_model(m_path);
  }
}

void mcs_nn::init_inlets_and_outlets() {
  std::string input_label; 
  for (int i(0); i < n_batches; i++) {
    input_label = "(multichannel) batch " + std::to_string(i) + "(" + std::to_string(m_model_in) + " dimensions)";
    m_inlets.push_back(
        std::make_unique<inlet<>>(this, input_label, "multichannelsignal"));
  }

  std::string output_label;
  for (int i(0); i < n_batches; i++) {
    output_label = "(multichannel) batch " + std::to_string(i) + "(" + std::to_string(m_model_out) + " dimensions)";
    m_outlets.push_back(
        std::make_unique<outlet<>>(this, output_label, "multichannelsignal"));
  }
}

void mcs_nn::init_buffers() {
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

void mcs_nn::init_process() {
    nn_base<mcs_nn, mc_operator<>>::init_process(); 
    if (m_use_thread) {
        m_compute_thread = std::make_unique<std::thread>(model_perform_async<mcs_nn>, this);
    }
}

void mcs_nn::perform(audio_bundle input, audio_bundle output) {
  auto chan_size = input.channel_count(); 
  auto vec_size = input.frame_count();

  // COPY INPUT TO CIRCULAR BUFFER
  for (int i(0); i < m_inlets.size(); i++) {
    for (int b(0); b < get_batches(); b++) {
      auto buf_idx = i * get_batches() + b;
      auto chan_idx = b * n_inlets + i;
      if (chan_idx < chan_size) {
        auto in = input.samples(b * n_inlets + i);
        m_in_buffer[buf_idx].put(in, vec_size);
      } 
    }
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
  for (int b(0); b < m_outlets.size(); b++) {
    for (int d(0); d < m_model_out; d++) {
      auto out = output.samples(d * get_batches() + b);
      m_out_buffer[b * m_model_out + d].get(out, vec_size);
    }
  }
}

long simplemc_multichanneloutputs(c74::max::t_object *x, long index,
                                  long count) {
  minwrap<mcs_nn> *ob = (minwrap<mcs_nn> *)(x);
  return ob->m_min_object.m_model_out;
}

long simplemc_inputchanged(c74::max::t_object *x, long index, long count) {
  minwrap<mcs_nn> *ob = (minwrap<mcs_nn> *)(x);
  auto chan_number = ob->m_min_object.m_model_in;
  if (chan_number != count) {
    c74::max::object_warn(
        x, (std::string("got " + std::to_string(count) + 
            " for " + std::to_string(chan_number) + 
            " model inputs").c_str())
    );
  }
  return true;
}



MIN_EXTERNAL(mcs_nn);