#include "../shared/nn_base.h"
#include "c74_min.h"


class nn: public nn_base<nn, vector_operator<>> {

public:
    MIN_DESCRIPTION{"Interface for deep learning models"};
    MIN_TAGS{"audio, deep learning, ai"};
    MIN_AUTHOR{"Antoine Caillon & Axel Chemla--Romeu-Santos"};
    MIN_RELATED{"nn.info, mc.nn~, mcs.nn~"};

    static std::string get_external_name() {
       return std::string("nn~");
    }
    nn(const atoms &args = {}) {
        init_external(args);
     }

    int get_sample_rate() override {
        return samplerate();
    }

    void perform(DspState &s, audio_bundle input, audio_bundle output) override;

    message<> maxclass_setup{
      this, "maxclass_setup",
      [this](const c74::min::atoms &args, const int inlet) -> c74::min::atoms {
        cout << "nn~ " << VERSION << " - torch " << TORCH_VERSION
             << " - 2023-2025 - Antoine Caillon & Axel Chemla--Romeu-Santos" << endl;
        cout << "visit https://www.github.com/acids-ircam" << endl;
        return {};
      }};

};


void nn::perform(DspState &s, audio_bundle input, audio_bundle output) {
  auto vec_size = input.frame_count();

  // COPY INPUT TO CIRCULAR BUFFER
  const int n_in = std::min<int>(input.channel_count(), s.buffer_in);
  for (int c(0); c < n_in; c++) {
    s.in_buffer[c].put(input.samples(c), vec_size);
  }

  if (s.buffer_in > 0 && s.in_buffer[0].full()) { // BUFFER IS FULL
    const int n_ins = std::min<int>(s.buffer_in, s.in_model.size());
    const int n_outs = std::min<int>(s.buffer_out, s.out_model.size());

    if (!m_use_thread) {
      // TRANSFER MEMORY BETWEEN INPUT CIRCULAR BUFFER AND MODEL BUFFER
      for (int c(0); c < n_ins; c++)
        s.in_buffer[c].get(s.in_model[c].get(), s.buffer_size);

      // CALL MODEL PERFORM IN CURRENT THREAD
      run_model(s);

      // TRANSFER MEMORY BETWEEN OUTPUT CIRCULAR BUFFER AND MODEL BUFFER
      for (int c(0); c < n_outs; c++)
        s.out_buffer[c].put(s.out_model[c].get(), s.buffer_size);

    } else if (m_result_available_lock.try_acquire()) {
      // TRANSFER MEMORY BETWEEN INPUT CIRCULAR BUFFER AND MODEL BUFFER
      for (int c(0); c < n_ins; c++)
        s.in_buffer[c].get(s.in_model[c].get(), s.buffer_size);

      // TRANSFER MEMORY BETWEEN OUTPUT CIRCULAR BUFFER AND MODEL BUFFER
      for (int c(0); c < n_outs; c++)
        s.out_buffer[c].put(s.out_model[c].get(), s.buffer_size);

      // SIGNAL PERFORM THREAD THAT DATA IS AVAILABLE
      m_data_available_lock.release();
    }
  }

  // COPY CIRCULAR BUFFER TO OUTPUT
  for (int c(0); c < output.channel_count(); c++) {
    auto out = output.samples(c);
    if (c < s.buffer_out) {
      s.out_buffer[c].get(out, vec_size);
    } else {
      std::fill(out, out + vec_size, 0.);
    }
  }
}


MIN_EXTERNAL(nn);
