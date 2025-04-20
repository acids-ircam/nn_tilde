#include "../shared/nn_base.h"
#include "c74_min.h"


template <typename nn_class>
void model_perform(nn_class* nn_instance) {
  std::vector<float *> in_model, out_model;
  for (int c(0); c < nn_instance->n_inlets; c++)
    in_model.push_back(nn_instance->m_in_model[c].get());
  for (int c(0); c < nn_instance->n_outlets; c++)
    out_model.push_back(nn_instance->m_out_model[c].get());
    
  if (nn_instance->wait_for_buffer_reset) {
    nn_instance->init_buffers(); 
  }

  nn_instance->m_model->perform(in_model, out_model, nn_instance->m_method, 
                                nn_instance->get_batches(), nn_instance->n_outlets, nn_instance->m_buffer_size);
}

template <typename nn_class>
void model_perform_async(nn_class* nn_instance) {
  while (!nn_instance->can_perform()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(REFRESH_THREAD_INTERVAL));
    if (nn_instance->m_should_stop_perform_thread) {
      return;
    }
  }
  std::vector<float *> in_model, out_model;
  if (nn_instance->wait_for_buffer_reset) {
    nn_instance->init_buffers(); 
  }
  
  for (int c(0); c < nn_instance->n_inlets; c++)
    in_model.push_back(nn_instance->m_in_model[c].get());
  for (int c(0); c < nn_instance->n_outlets; c++)
    out_model.push_back(nn_instance->m_out_model[c].get());

  while (!nn_instance->m_should_stop_perform_thread) {
    if (nn_instance->m_data_available_lock.try_acquire_for(
            std::chrono::milliseconds(REFRESH_THREAD_INTERVAL))) {
        if (nn_instance->wait_for_buffer_reset) {
          nn_instance->init_buffers(); 
        }
        if (nn_instance->had_buffer_reset) {
          in_model.clear(); 
          for (int c(0); c < nn_instance->m_model_in * nn_instance->get_batches(); c++) {
            in_model.push_back(nn_instance->m_in_model[c].get());
          }
          out_model.clear(); 
          for (int c(0); c < nn_instance->m_model_out * nn_instance->get_batches(); c++) {
            out_model.push_back(nn_instance->m_out_model[c].get());
          }
        }
        nn_instance->m_model->perform(in_model, 
                                      out_model,
                                      nn_instance->m_method, 
                                      nn_instance->get_batches(),
                                      nn_instance->m_out_model.size() / nn_instance->get_batches(),
                                      nn_instance->m_buffer_size);
        nn_instance->m_result_available_lock.release();
    }
  }
}

long simplemc_multichanneloutputs(c74::max::t_object *x, long index,
                                  long count);
long simplemc_inputchanged(c74::max::t_object *x, long index, long count);


class mc_nn: public nn_base<mc_nn, mc_operator<>> {

public:
    MIN_DESCRIPTION{"Multi-channel interface for deep learning models"};
    MIN_TAGS{"audio, deep learning, ai"};
    MIN_AUTHOR{"Antoine Caillon & Axel Chemla--Romeu-Santos"};
    MIN_RELATED{"nn.info, nn~, mcs.nn~"};

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
        // init_buffers(); 
        init_process();
    } 
    void perform(audio_bundle input, audio_bundle output) override;

    // channel handling
    std::vector<int> channel_map;
    int n_batches_arg = 0;
    int get_batches(); 
    int get_batches_out(); 

    void init_inputs_and_outputs(const atoms& atoms) override; 
    void init_inlets_and_outlets() override; 
    void init_buffers() override; 
    void init_process() override; 
    bool init_buffers_at_init() override { return true; }
    bool update_channel_map(const long& index, const long& count); 

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

    attribute<int> chans_attr {
      this, 
      "chans", 
      0,
      description{"set a fixed number of output channels"}, 
      setter{
        MIN_FUNCTION {
          if (args.size() == 0)
            return args; 
          int in_chans = args[0];
          if (in_chans > 0) {
            n_batches_arg = in_chans;
            DEBUG_PRINT("setting out channels to %d", in_chans);
          }
          return args;
        }
      }};
};

int mc_nn::get_batches() {
  if (channel_map.size() > 0) {
    return *std::max_element(channel_map.begin(), channel_map.end());
  } else {
    return 1; 
  }
}

int mc_nn::get_batches_out() {
  if (n_batches_arg > 0) {
    return n_batches_arg; 
  } else {
    return get_batches(); 
  }
}

void mc_nn::init_inputs_and_outputs(const atoms& args)  {
    nn_base::init_inputs_and_outputs(args);
    for (int i(0); i < m_model_in; i++)
        channel_map.push_back(1);
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
      input_label = "(multichannel) model input " + std::to_string(i);
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
      output_label = "(multichannel) model output " + std::to_string(i);
    }
    m_outlets.push_back(
        std::make_unique<outlet<>>(this, output_label, "multichannelsignal"));
  }
}

void mc_nn::init_buffers() {
  if (channel_map.size() == 0) {
    for (int i(0); i < n_inlets; i++)
        channel_map.push_back(1);
  }

  update_method(); 

  // if (m_buffer_size == -1) {
  //   // NO THREAD MODE
  //   m_buffer_size = DEFAULT_BUFFER_SIZE;
  // }
  // if (m_buffer_size < m_higher_ratio) {
  //   cerr << "buffer size too small, switching to " << m_buffer_size << endl;
  //   m_buffer_size = m_higher_ratio;
  // } else {
  //   m_buffer_size = power_ceil(m_buffer_size);
  // }

  m_buffer_in = n_inlets * get_batches();
  if (m_in_buffer.get() != nullptr) { m_in_buffer.release(); }
  m_in_buffer = std::make_unique<circular_buffer<double, float>[]>(m_buffer_in); 
  for (int i = 0; i < m_buffer_in; i++) {
    m_in_buffer[i].initialize(m_buffer_size);
  }

  m_buffer_out = n_outlets * get_batches_out();
  if (m_out_buffer.get() == nullptr) { m_out_buffer.release(); }
  m_out_buffer = std::make_unique<circular_buffer<float, double>[]>(m_buffer_out); 
  for (int i = 0; i < m_buffer_out; i++) {
    m_out_buffer[i].initialize(m_buffer_size);
  }

  m_in_model.clear(); 
  for (int i = 0; i < m_model_in * get_batches(); i++) {
    m_in_model.push_back(std::make_unique<float[]>(m_buffer_size));
    std::fill(m_in_model[i].get(), m_in_model[i].get() + m_buffer_size, 0.); 
  }

  m_out_model.clear(); 
  for (int i = 0; i < m_model_out * get_batches(); i++) {
    m_out_model.push_back(std::make_unique<float[]>(m_buffer_size));
    std::fill(m_out_model[i].get(), m_out_model[i].get() + m_buffer_size, 0.); 
  }

  wait_for_buffer_reset = false; 
  had_buffer_reset = true; 
  buffer_initialised = true; 
}

void mc_nn::init_process() {
    nn_base<mc_nn, mc_operator<>>::init_process(); 
    if (m_use_thread) {
        m_compute_thread = std::make_unique<std::thread>(model_perform_async<mc_nn>, this);
    }
}

void mc_nn::perform(audio_bundle input, audio_bundle output) {
  auto vec_size = input.frame_count();
  auto chan_size = input.channel_count(); 

  // COPY INPUT TO CIRCULAR BUFFER
  int current_batch = 0; 
  int current_chan = 0; 
  int n_batches = get_batches(); 

  for (int c_in(0); c_in < chan_size; c_in++) {
    auto in = input.samples(c_in); 
    auto buf_idx = current_batch * n_batches + current_chan; 
    if (buf_idx < m_buffer_in) {
      m_in_buffer[buf_idx].put(in, vec_size);
    }
    current_chan++; 
    if (current_chan >= channel_map[current_batch]) {
      current_batch++;
      current_chan=0; 
    }
  }

  if (m_in_buffer[0].full()) { // BUFFER IS FULL
    if (!m_use_thread) {
      // TRANSFER MEMORY BETWEEN INPUT CIRCULAR BUFFER AND MODEL BUFFER
      auto n_ins = std::min(n_inlets, m_model_in);
      for (int b(0); b < n_batches; b++) {
        for (int c(0); c < n_ins; c++) {
          auto buf_idx = c * get_batches() + b; 
          if ((buf_idx < m_buffer_in) && (c * n_batches + b < m_in_model.size()))
            m_in_buffer[buf_idx].get(m_in_model[c * n_batches + b].get(), m_buffer_size);
        }
      }

      // CALL MODEL PERFORM IN CURRENT THREAD
      model_perform(this);

      // TRANSFER MEMORY BETWEEN OUTPUT CIRCULAR BUFFER AND MODEL BUFFER
      auto n_outs = std::min(n_outlets, m_model_out);
      auto n_batches_out = std::min(get_batches_out(), n_batches);
      for (int b(0); b < n_batches_out; b++) {
        for (int c(0); c < n_outs; c++) {
          auto b_idx = c * get_batches_out() + b;
          if ((b_idx < m_buffer_out) && (b * m_model_out + c < m_out_model.size()))
            m_out_buffer[b_idx].put(m_out_model[b * m_model_out + c].get(), m_buffer_size);
        }
      }

    } else if (m_result_available_lock.try_acquire()) {
      
      // TRANSFER MEMORY BETWEEN INPUT CIRCULAR BUFFER AND MODEL BUFFER
      auto n_ins = std::min(n_inlets, m_model_in);
      for (int b(0); b < n_batches; b++) {
        for (int c(0); c < n_ins; c++) {
          auto buf_idx = c * get_batches() + b; 
          if ((buf_idx < m_buffer_in) && (c * n_batches + b < m_in_model.size()))
            m_in_buffer[buf_idx].get(m_in_model[c * n_batches + b].get(), m_buffer_size);
        }
      }

      // TRANSFER MEMORY BETWEEN OUTPUT CIRCULAR BUFFER AND MODEL BUFFER
      auto n_outs = std::min(n_outlets, m_model_out);
      auto n_batches_out = std::min(get_batches_out(), n_batches);
      for (int b(0); b < n_batches_out; b++) {
        for (int c(0); c < n_outs; c++) {
          auto b_idx = c * get_batches_out() + b;
          if ((b_idx < m_buffer_out) && (b * m_model_out + c < m_out_model.size()))
            m_out_buffer[b_idx].put(m_out_model[b * m_model_out + c].get(), m_buffer_size);
        }
      }

      // SIGNAL PERFORM THREAD THAT DATA IS AVAILABLE
      m_data_available_lock.release();
    }
  }

  // COPY CIRCULAR BUFFER TO OUTPUT
  for (int b(0); b < output.channel_count() ; b++) {
    if (b < m_buffer_out) {
      auto out = output.samples(b); 
      m_out_buffer[b].get(out, vec_size);
    }
  }
  
}

bool mc_nn::update_channel_map(const long& index, const long& count) {
  bool needs_refresh = false; 
  if (channel_map.size() == 0) {
    for (int i(0); i < n_inlets; i++)
        channel_map.push_back(1);
  }
  if (index > channel_map.size()) {
    return false; 
  }
  if (channel_map[index] != count) {
    channel_map[index] = count; 
    wait_for_buffer_reset = true; 
  }
  return true; 
}


long simplemc_multichanneloutputs(c74::max::t_object *x, long index,
                                  long count) {
  minwrap<mc_nn> *ob = (minwrap<mc_nn> *)(x);
  return ob->m_min_object.get_batches_out();
}

long simplemc_inputchanged(c74::max::t_object *x, long index, long count) {
  minwrap<mc_nn> *ob = (minwrap<mc_nn> *)(x);
  ob->m_min_object.update_channel_map(index, count);
  return true; 
}



MIN_EXTERNAL(mc_nn);