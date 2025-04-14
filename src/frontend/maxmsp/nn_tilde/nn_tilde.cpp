#include "../shared/nn_base.h"
#include "c74_min.h"

template <typename nn_class>
void model_perform(nn_class* nn_instance) {
  std::vector<float *> in_model, out_model;
  for (int c(0); c < nn_instance->m_model_in; c++)
    in_model.push_back(nn_instance->m_in_model[c].get());
  for (int c(0); c < nn_instance->m_model_out; c++)
    out_model.push_back(nn_instance->m_out_model[c].get());

  if (nn_instance->had_buffer_reset) {
    nn_instance->had_buffer_reset = false; 
  }

  nn_instance->m_model->perform(in_model, out_model, 
                                nn_instance->m_method, 
                                1, nn_instance->m_out_model.size(), nn_instance->m_buffer_size);
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

  if (nn_instance->wait_for_buffer_reset) {
    nn_instance->init_buffers(); 
  }

  for (int c(0); c < nn_instance->m_model_in; c++)
    in_model.push_back(nn_instance->m_in_model[c].get());
  for (int c(0); c < nn_instance->m_model_out; c++)
    out_model.push_back(nn_instance->m_out_model[c].get());

  while (!nn_instance->m_should_stop_perform_thread) {
    if (nn_instance->m_data_available_lock.try_acquire_for(
            std::chrono::milliseconds(REFRESH_THREAD_INTERVAL))) {
          if (nn_instance->wait_for_buffer_reset) {
            nn_instance->init_buffers(); 
          }
          if (nn_instance->had_buffer_reset) {
            in_model.clear(); 
            for (int c(0); c < nn_instance->m_model_in; c++) {
              in_model.push_back(nn_instance->m_in_model[c].get());
            }
            out_model.clear(); 
            for (int c(0); c < nn_instance->m_model_out; c++) {
              out_model.push_back(nn_instance->m_out_model[c].get());
            }
            nn_instance->had_buffer_reset = false; 
          }
          nn_instance->m_model->perform(in_model, out_model,
                                      nn_instance->m_method, 
                                      1, nn_instance->m_out_model.size(), nn_instance->m_buffer_size);
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
      DEBUG_PRINT("initializing model");
      init_model(); 
      DEBUG_PRINT("initializing downloader");
      init_downloader(); 
      if (!args.size()) { return; } 
      DEBUG_PRINT("initializing inputs & outputs");
      init_inputs_and_outputs(args); 
      DEBUG_PRINT("initializing inlets & outlets");
      init_inlets_and_outlets(); 
      DEBUG_PRINT("initializing buffers");
      init_buffers(); 
      DEBUG_PRINT("initializing process"); 
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
        if (wait_for_buffer_reset) {
              init_buffers();
          }
          // TRANSFER MEMORY BETWEEN INPUT CIRCULAR BUFFER AND MODEL BUFFER
          auto n_ins = std::min(n_inlets, m_model_in);
          for (int c(0); c < n_ins; c++)
            m_in_buffer[c].get(m_in_model[c].get(), m_buffer_size);

          // CALL MODEL PERFORM IN CURRENT THREAD
          model_perform<nn>(this);
          // TRANSFER MEMORY BETWEEN OUTPUT CIRCULAR BUFFER AND MODEL BUFFER
          auto n_outs = std::min(n_outlets, m_model_out);
          for (int c(0); c < n_outs; c++)
            m_out_buffer[c].put(m_out_model[c].get(), m_buffer_size);
      } else { 
        if (m_result_available_lock.try_acquire()) {
          // TRANSFER MEMORY BETWEEN INPUT CIRCULAR BUFFER AND MODEL BUFFER
          if (wait_for_buffer_reset) {
              init_buffers();
          }
          auto n_ins = std::min(n_inlets, m_model_in);
          for (int c(0); c < n_ins; c++)
            m_in_buffer[c].get(m_in_model[c].get(), m_buffer_size);

          // TRANSFER MEMORY BETWEEN OUTPUT CIRCULAR BUFFER AND MODEL BUFFER
          auto n_outs = std::min(n_outlets, m_model_out);
          for (int c(0); c < n_outs; c++)
            m_out_buffer[c].put(m_out_model[c].get(), m_buffer_size);


          // SIGNAL PERFORM THREAD THAT DATA IS AVAILABLE
          m_data_available_lock.release();
        } 
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