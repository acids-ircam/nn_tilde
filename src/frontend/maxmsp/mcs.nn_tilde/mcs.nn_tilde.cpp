#include "../shared/nn_base.h"
#include "c74_min.h"


template <typename nn_class>
void model_perform(nn_class* nn_instance) {
  std::vector<float *> in_model, out_model;
  for (int c(0); c < nn_instance->m_model_in * nn_instance->n_batches; c++)
    in_model.push_back(nn_instance->m_in_model[c].get());
  for (int c(0); c < nn_instance->m_model_out * nn_instance->n_batches; c++)
    out_model.push_back(nn_instance->m_out_model[c].get());

  if (nn_instance->had_buffer_reset) {
    nn_instance->had_buffer_reset = false; 
  }

  nn_instance->m_model->perform(in_model, out_model, nn_instance->m_method, 
                                nn_instance->n_inlets, nn_instance->m_model_out, nn_instance->m_buffer_size);
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
  if (nn_instance->wait_for_buffer_reset) {
    nn_instance->init_buffers(); 
  }
  for (int c(0); c < nn_instance->m_model_in * nn_instance->n_batches; c++)
    in_model.push_back(nn_instance->m_in_model[c].get());
  for (int c(0); c < nn_instance->m_model_out * nn_instance->n_batches; c++)
    out_model.push_back(nn_instance->m_out_model[c].get());

  while (!nn_instance->m_should_stop_perform_thread) {
    if (nn_instance->wait_for_buffer_reset) {
      nn_instance->init_buffers(); 
    }
    if (nn_instance->had_buffer_reset) {
      in_model.clear(); 
      for (int c(0); c < nn_instance->m_model_in * nn_instance->n_batches; c++) {
        in_model.push_back(nn_instance->m_in_model[c].get());
      }
      out_model.clear(); 
      for (int c(0); c < nn_instance->m_model_out * nn_instance->n_batches; c++) {
        out_model.push_back(nn_instance->m_out_model[c].get());
      }
      nn_instance->had_buffer_reset = false; 
    }

    if (nn_instance->m_data_available_lock.try_acquire_for(
            std::chrono::milliseconds(REFRESH_THREAD_INTERVAL))) {
      nn_instance->m_model->perform(in_model, out_model, nn_instance->m_method, 
                                    nn_instance->n_inlets, nn_instance->m_model_out, nn_instance->m_buffer_size);
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
    int n_mc_inputs() {
      return std::accumulate(channel_map.begin(), channel_map.end(), 0);
    } 
    bool check_inputs(); 
    std::vector<long> channel_map;

    void init_inputs_and_outputs(const atoms& atoms) override; 
    void init_inlets_and_outlets() override; 
    void init_buffers() override; 
    void init_process() override; 
    void update_method(const std::string &method) override;
    bool update_channel_map(const long& index, const long& count); 
    int m_effective_model_out; 
    void update_effective_model_out() {
      // if method is changed, multi channels outputs will stay the same. 
      m_effective_model_out = (int)m_model_out; 
    }

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
  return *std::max_element(channel_map.begin(), channel_map.end());
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
    channel_map = std::vector<long>(n_batches, 1); 
    load_model(m_path);
  }
}

bool mcs_nn::update_channel_map(const long& index, const long& count)
{
  if (channel_map[index] != count) {
    channel_map[index] = count;
    wait_for_buffer_reset = true; 
    if (count != m_model_in) {
      return false;
    } else {
      return true;
    }
  }
  return true; 
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
  n_inlets = n_batches; 
  n_outlets = n_batches; 
}

void mcs_nn::init_buffers() {
  if (m_buffer_size == -1) {
    // NO THREAD MODE
    m_buffer_size = DEFAULT_BUFFER_SIZE;
  }
  if (m_buffer_size == 0) {
    m_use_thread = false; 
    m_buffer_size = m_higher_ratio; 
  } else {
    if (m_buffer_size < m_higher_ratio) {
      cerr << "buffer size too small, switching to " << m_buffer_size << endl;
      m_buffer_size = m_higher_ratio;
    } else {
      m_buffer_size = power_ceil(m_buffer_size);
    }
  }

 if (m_in_buffer.get() != nullptr) { m_in_buffer.release(); }
  m_in_buffer = std::make_unique<circular_buffer<double, float>[]>(n_mc_inputs());
  if (m_in_buffer.get()->max_size() < m_buffer_size) {
    for (int i = 0; i < n_mc_inputs(); i++) {
      m_in_buffer[i].initialize(m_buffer_size);
    }
  }
  m_buffer_in = n_mc_inputs(); 

  if (m_out_buffer.get() == nullptr) { m_out_buffer.release(); }
  m_out_buffer = std::make_unique<circular_buffer<float, double>[]>(n_outlets * m_model_out);
  if (m_out_buffer.get()->max_size() < m_buffer_size) {
    for (int i = 0; i < n_outlets * m_model_out; i++) {
      m_out_buffer[i].initialize(m_buffer_size);
    }
  }
  m_buffer_out = n_outlets * m_model_out;

  m_in_model.clear(); 
  for (int i = 0; i < m_model_in * n_inlets; i++) {
    m_in_model.push_back(std::make_unique<float[]>(m_buffer_size));
    std::fill(m_in_model[i].get(), m_in_model[i].get() + m_buffer_size, 0.); 
  }

  m_out_model.clear(); 
  for (int i = 0; i < m_model_out * n_outlets; i++) {
    m_out_model.push_back(std::make_unique<float[]>(m_buffer_size));
    std::fill(m_out_model[i].get(), m_out_model[i].get() + m_buffer_size, 0.); 
  }

  wait_for_buffer_reset = false; 
  had_buffer_reset = true; 
}

void mcs_nn::update_method(const std::string &method) {
  if (!m_model->is_loaded()) {
    cerr << "no model is set yet" << endl;
    return; 
  }
  if (m_model->has_method(method)) {
    m_method = method; 
    auto params = m_model->get_method_params(m_method);
    // input parameters
    m_model_in = params[0];
    if (n_inlets == -1) {
      n_inlets = m_model_in;
    } 
    m_in_ratio = params[1];

    // output parameters
    m_model_out = params[2];
    if (n_outlets == -1) {
      n_outlets = params[2];
    }
    m_out_ratio = params[3]; 
  } else {
    cerr << "method " << method << " not present in model" << endl;
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

  if (m_ready) {
    if (wait_for_buffer_reset) {
        init_buffers();
    }

    int current_batch = 0; 
    int current_channel = 0; 
    for (int in_c(0); in_c < chan_size; in_c++) {
      m_in_buffer[in_c].put(input.samples(in_c), vec_size);
    }

    if (m_in_buffer[0].full()) { // BUFFER IS FULL
      if (!m_use_thread) {
        // TRANSFER MEMORY BETWEEN INPUT CIRCULAR BUFFER AND MODEL BUFFER
        int current_chan = 0; 
        int current_batch = 0; 
        for (int i(0); i < chan_size; i++) {
          if (current_chan < m_model_in) {
            auto c_idx = current_chan * n_batches + current_batch;
            m_in_buffer[i].get(m_in_model[c_idx].get(), m_buffer_size);
          }
          current_chan++; 
          if (current_chan >= channel_map[current_batch]) {
            current_batch += 1;
            current_chan = 0;
          }
        }

        // CALL MODEL PERFORM IN CURRENT THREAD
        model_perform(this);

        // TRANSFER MEMORY BETWEEN OUTPUT CIRCULAR BUFFER AND MODEL BUFFER
        auto current_idx = 0; 
        for (int c(0); c < n_outlets * m_model_out; c++){ 
            m_out_buffer[c].put(m_out_model[c].get(), m_buffer_size);
        }

      } else if (m_result_available_lock.try_acquire()) {

        // TRANSFER MEMORY BETWEEN INPUT CIRCULAR BUFFER AND MODEL BUFFER
        // for (int c(0); c < m_model_in * get_batches(); c++)
        int current_chan = 0; 
        int current_batch = 0; 
        for (int i(0); i < chan_size; i++) {
          if (current_chan < m_model_in) {
            auto c_idx = current_chan * n_batches + current_batch;
            m_in_buffer[i].get(m_in_model[c_idx].get(), m_buffer_size);
          }
          current_chan++; 
          if (current_chan >= channel_map[current_batch]) {
            current_batch += 1;
            current_chan = 0;
          }
        }

        // TRANSFER MEMORY BETWEEN OUTPUT CIRCULAR BUFFER AND MODEL BUFFER
        auto current_idx = 0; 
        for (int c(0); c < n_outlets * m_model_out; c++){ 
            m_out_buffer[c].put(m_out_model[c].get(), m_buffer_size);
        }

        // SIGNAL PERFORM THREAD THAT DATA IS AVAILABLE
        m_data_available_lock.release();
      }
    }

    // COPY CIRCULAR BUFFER TO OUTPUT
    // for (int c(0); c < n_outlets * m_model_out; c++) {
    //   auto out = output.samples(c); 
    //   m_out_buffer[c].get(out, vec_size);
    // 
    // }
    for (int b(0); b < n_outlets; b++) {
      for (int c(0); c < m_model_out; c++) {
        auto out = output.samples(b * m_effective_model_out + c);
        m_out_buffer[b * m_model_out + c].get(out, vec_size); 
      }
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
  auto is_full = ob->m_min_object.update_channel_map(index, count);
  ob->m_min_object.update_effective_model_out();

  if (!is_full) {
    auto n_channels = ob->m_min_object.m_model_in; 
    c74::max::object_warn(
        x, (std::string("got " + std::to_string(count) + 
            " for " + std::to_string(n_channels) + 
            " model inputs").c_str())
    );
  }
  return true;
}



MIN_EXTERNAL(mcs_nn);