#include "../shared/nn_base.h"
#include "c74_min.h"


template <typename nn_class>
void model_perform(nn_class* nn_instance) {
  std::vector<float *> in_model, out_model;
  for (int c(0); c < nn_instance->n_inlets; c++)
    in_model.push_back(nn_instance->m_in_model[c].get());
  for (int c(0); c < nn_instance->n_outlets; c++)
    out_model.push_back(nn_instance->m_out_model[c].get());

  nn_instance->m_model->perform(in_model, out_model, nn_instance->m_buffer_size,
                                nn_instance->m_method, 1);
}

template <typename nn_class>
void model_perform_async(nn_class *nn_instance) {
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
        nn_instance->m_model->perform(in_model, out_model,
                                      nn_instance->m_buffer_size,
                                      nn_instance->m_method, 1);
        nn_instance->m_result_available_lock.release();
    }
  }
}



class nn: public nn_base<nn, vector_operator<>> {

public:
    MIN_DESCRIPTION{"Interface for deep learning models"};
    MIN_TAGS{"audio, deep learning, ai"};
    MIN_AUTHOR{"Antoine Caillon & Axel Chemla--Romeu-Santos"};

    static std::string get_external_name() { return "nn~";} 
    nn(const atoms &args = {}) {
        init_external(args); 
     }

    int get_sample_rate() override {
        return samplerate(); 
    }

    void init_process() override { 
        nn_base<nn>::init_process(); 
        if (m_use_thread) {
            m_compute_thread = std::make_unique<std::thread>(model_perform_async<nn>, this);
        }
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

    message<> maxclass_setup{
      this, "maxclass_setup",
      [this](const c74::min::atoms &args, const int inlet) -> c74::min::atoms {
        cout << "nn~ " << VERSION << " - torch " << TORCH_VERSION
             << " - 2023-2025 - Antoine Caillon & Axel Chemla--Romeu-Santos" << endl;
        cout << "visit https://www.github.com/acids-ircam" << endl;
        return {};
      }};
      
};


void nn::perform(audio_bundle input, audio_bundle output) {
  auto vec_size = input.frame_count();

  if (m_ready) {
    // COPY INPUT TO CIRCULAR BUFFER
    for (int c(0); c < input.channel_count(); c++) {
      auto in = input.samples(c);
      m_in_buffer[c].put(in, vec_size);
    }

    if (m_in_buffer[0].full()) { // BUFFER IS FULL
      if (!m_use_thread) {
          // TRANSFER MEMORY BETWEEN INPUT CIRCULAR BUFFER AND MODEL BUFFER
          for (int c(0); c < n_inlets; c++)
            m_in_buffer[c].get(m_in_model[c].get(), m_buffer_size);

          // CALL MODEL PERFORM IN CURRENT THREAD
          model_perform<nn>(this);

          // TRANSFER MEMORY BETWEEN OUTPUT CIRCULAR BUFFER AND MODEL BUFFER
          for (int c(0); c < n_outlets; c++)
            m_out_buffer[c].put(m_out_model[c].get(), m_buffer_size);
      } else if (m_result_available_lock.try_acquire()) {
        // TRANSFER MEMORY BETWEEN INPUT CIRCULAR BUFFER AND MODEL BUFFER
        for (int c(0); c < n_inlets; c++)
          m_in_buffer[c].get(m_in_model[c].get(), m_buffer_size);

        // TRANSFER MEMORY BETWEEN OUTPUT CIRCULAR BUFFER AND MODEL BUFFER
        for (int c(0); c < n_outlets; c++)
          m_out_buffer[c].put(m_out_model[c].get(), m_buffer_size);

        // SIGNAL PERFORM THREAD THAT DATA IS AVAILABLE
        m_data_available_lock.release();
      }
    }
  

    // COPY CIRCULAR BUFFER TO OUTPUT
    for (int c(0); c < output.channel_count(); c++) {
      auto out = output.samples(c);
      m_out_buffer[c].get(out, vec_size);
    }
  }
}


MIN_EXTERNAL(nn);