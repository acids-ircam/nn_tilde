#include "../shared/nn_base.h"
#include "c74_min.h"


template <typename nn_class>
void model_perform(nn_class* nn_instance) {
  std::vector<float *> in_model, out_model;
  for (int c(0); c < nn_instance->n_inlets; c++)
    in_model.push_back(nn_instance->m_in_model[c].get());
  for (int c(0); c < nn_instance->n_outlets; c++)
    out_model.push_back(nn_instance->m_out_model[c].get());
  if (nn_instance->had_buffer_reset) {
    nn_instance->had_buffer_reset = false; 
  }

  nn_instance->m_model->perform(in_model, out_model, nn_instance->m_method, 
                                nn_instance->get_batches(), nn_instance->n_outlets, nn_instance->m_buffer_size);
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
    if (nn_instance->m_data_available_lock.try_acquire_for(
            std::chrono::milliseconds(REFRESH_THREAD_INTERVAL))) {
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
    std::vector<int> channel_map;
    int get_batches(); 

    void init_inputs_and_outputs(const atoms& atoms) override; 
    void init_inlets_and_outlets() override; 
    void init_buffers() override; 
    void init_process() override; 
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
};

int mc_nn::get_batches() {
  return *std::max_element(channel_map.begin(), channel_map.end());
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

 if (m_in_buffer.get() != nullptr) { m_in_buffer.release(); }
  m_in_buffer = std::make_unique<circular_buffer<double, float>[]>(n_inlets * get_batches());
  if (m_in_buffer.get()->max_size() < m_buffer_size) {
    for (int i = 0; i < n_inlets * get_batches(); i++) {
      m_in_buffer[i].initialize(m_buffer_size);
    }
  }
  m_buffer_in = n_inlets * get_batches();

  if (m_out_buffer.get() == nullptr) { m_out_buffer.release(); }
  m_out_buffer = std::make_unique<circular_buffer<float, double>[]>(n_outlets * get_batches());
  if (m_out_buffer.get()->max_size() < m_buffer_size) {
    for (int i = 0; i < n_outlets * get_batches(); i++) {
      m_out_buffer[i].initialize(m_buffer_size);
    }
  }
  m_buffer_out = n_outlets * get_batches();

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

  if (m_ready) {
    if (wait_for_buffer_reset) {
      init_buffers();
    }
    // COPY INPUT TO CIRCULAR BUFFER
    int current_batch = 0; 
    int current_chan = 0; 
    int n_batches = get_batches(); 
    for (int c_in(0); c_in < chan_size; c_in++) {
      auto in = input.samples(c_in); 
      auto buf_idx = current_batch * n_batches + current_chan; 
      if (buf_idx < n_inlets * n_batches) {
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
        for (int c(0); c < n_ins * get_batches(); c++)
          m_in_buffer[c].get(m_in_model[c].get(), m_buffer_size);

        // CALL MODEL PERFORM IN CURRENT THREAD
        model_perform(this);

        // TRANSFER MEMORY BETWEEN OUTPUT CIRCULAR BUFFER AND MODEL BUFFER
        auto n_outs = std::min(n_outlets, m_model_out);
        for (int c(0); c < n_outs * get_batches(); c++)
          m_out_buffer[c].put(m_out_model[c].get(), m_buffer_size);

      } else if (m_result_available_lock.try_acquire()) {
        
        // TRANSFER MEMORY BETWEEN INPUT CIRCULAR BUFFER AND MODEL BUFFER
        auto n_ins = std::min(n_inlets, m_model_in);
        for (int b(0); b < n_batches; b++) {
          for (int c(0); c < n_ins; c++) {
            m_in_buffer[b * n_ins + c].get(m_in_model[b * n_ins + c].get(), m_buffer_size);
          }
        }

        // TRANSFER MEMORY BETWEEN OUTPUT CIRCULAR BUFFER AND MODEL BUFFER
        auto n_outs = std::min(n_outlets, m_model_out);
        for (int b(0); b < n_batches; b++) {
          for (int c(0); c < n_outs; c++) {
            m_out_buffer[b * n_outs + c].put(m_out_model[b * m_model_out + c].get(), m_buffer_size);
          }
        }

        // SIGNAL PERFORM THREAD THAT DATA IS AVAILABLE
        m_data_available_lock.release();
      }
    }

    // COPY CIRCULAR BUFFER TO OUTPUT
    auto n_outs = std::min(m_model_out, n_outlets); 
    for (int b(0); b < n_batches; b++) {
      for (int d(0); d < n_outs; d++) {
        auto out = output.samples(d * n_batches + b); 
        m_out_buffer[b * n_outs + d].get(out, vec_size);
      }
    }
  }
}

bool mc_nn::update_channel_map(const long& index, const long& count) {
  bool needs_refresh = false; 
  if (channel_map[index] != count) {
    channel_map[index] = count; 
    wait_for_buffer_reset = true; 
  }
  return true; 
}


long simplemc_multichanneloutputs(c74::max::t_object *x, long index,
                                  long count) {
  minwrap<mc_nn> *ob = (minwrap<mc_nn> *)(x);
  return ob->m_min_object.get_batches();
}

long simplemc_inputchanged(c74::max::t_object *x, long index, long count) {
  minwrap<mc_nn> *ob = (minwrap<mc_nn> *)(x);
  if (ob->m_min_object.channel_map[index] != count) {
    ob->m_min_object.update_channel_map(index, count);
  }
  return true; 
}



MIN_EXTERNAL(mc_nn);