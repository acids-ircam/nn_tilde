#include "../shared/nn_base.h"
#include "c74_min.h"


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

    void perform(DspState &s, audio_bundle input, audio_bundle output) override;
    std::unique_ptr<DspState> build_state() override;

    // channel handling
    std::vector<int> channel_map;
    int n_batches_arg = 0;
    int get_batches(); 
    int get_batches_out(); 

    void init_inputs_and_outputs(const atoms& atoms) override; 
    void init_inlets_and_outlets() override; 
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
            if (m_is_backend_init)
              request_rebuild();
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

std::unique_ptr<DspState> mc_nn::build_state() {
  auto state = std::make_unique<DspState>();
  if (!m_ready || !m_model || !m_model->is_loaded())
    return state;
  auto params = m_model->get_method_params(m_method);
  if (params.size() < 4)
    return state;

  if (channel_map.size() == 0) {
    for (int i(0); i < n_inlets; i++)
        channel_map.push_back(1);
  }
  const int batches = get_batches();
  const int batches_out = get_batches_out();

  state->method = m_method;
  state->model_in = params[0];
  state->model_out = params[2];
  state->buffer_size = effective_buffer_size();
  state->n_batches = batches;
  state->n_out_channels = state->model_out;
  state->batches_out = batches_out;
  state->channel_map.assign(channel_map.begin(), channel_map.end());
  state->allocate(n_inlets * batches, n_outlets * batches_out,
                  state->model_in * batches, state->model_out * batches);
  state->valid = true;
  return state;
}

void mc_nn::perform(DspState &s, audio_bundle input, audio_bundle output) {
  auto vec_size = input.frame_count();
  auto chan_size = input.channel_count(); 
  const int n_batches = s.n_batches;
  const int n_map = s.channel_map.size();

  // COPY INPUT TO CIRCULAR BUFFER
  // (input channels are ordered inlet by inlet; buffer idx = inlet * n_batches + chan)
  int current_inlet = 0; 
  int current_chan = 0; 
  for (int c_in(0); c_in < chan_size && current_inlet < n_map; c_in++) {
    auto buf_idx = current_inlet * n_batches + current_chan; 
    if (buf_idx < s.buffer_in) {
      s.in_buffer[buf_idx].put(input.samples(c_in), vec_size);
    }
    current_chan++; 
    if (current_chan >= s.channel_map[current_inlet]) {
      current_inlet++;
      current_chan = 0; 
    }
  }

  if (s.buffer_in > 0 && s.in_buffer[0].full()) { // BUFFER IS FULL
    const int n_in_ports = n_batches > 0 ? s.buffer_in / n_batches : 0;
    const int n_out_ports = s.batches_out > 0 ? s.buffer_out / s.batches_out : 0;

    auto transfer_in = [&]() {
      auto n_ins = std::min(n_in_ports, s.model_in);
      for (int b(0); b < n_batches; b++) {
        for (int c(0); c < n_ins; c++) {
          auto idx = c * n_batches + b; 
          if ((idx < s.buffer_in) && (idx < (int)s.in_model.size()))
            s.in_buffer[idx].get(s.in_model[idx].get(), s.buffer_size);
        }
      }
    };
    auto transfer_out = [&]() {
      auto n_outs = std::min(n_out_ports, s.model_out);
      auto n_batches_out = std::min(s.batches_out, n_batches);
      for (int b(0); b < n_batches_out; b++) {
        for (int c(0); c < n_outs; c++) {
          auto b_idx = c * s.batches_out + b;
          auto m_idx = b * s.model_out + c;
          if ((b_idx < s.buffer_out) && (m_idx < (int)s.out_model.size()))
            s.out_buffer[b_idx].put(s.out_model[m_idx].get(), s.buffer_size);
        }
      }
    };

    if (!m_use_thread) {
      transfer_in();
      run_model(s);
      transfer_out();
    } else if (m_result_available_lock.try_acquire()) {
      transfer_in();
      transfer_out();
      // SIGNAL PERFORM THREAD THAT DATA IS AVAILABLE
      m_data_available_lock.release();
    }
  }

  // COPY CIRCULAR BUFFER TO OUTPUT
  for (int b(0); b < output.channel_count() ; b++) {
    auto out = output.samples(b); 
    if (b < s.buffer_out) {
      s.out_buffer[b].get(out, vec_size);
    } else {
      std::fill(out, out + vec_size, 0.);
    }
  }
}

bool mc_nn::update_channel_map(const long& index, const long& count) {
  bool needs_refresh = false; 
  if (channel_map.size() == 0) {
    for (int i(0); i < n_inlets; i++)
        channel_map.push_back(1);
  }
  if (index < 0 || index >= (long)channel_map.size()) {
    return false; 
  }
  if (channel_map[index] != count) {
    channel_map[index] = count; 
    request_rebuild();
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