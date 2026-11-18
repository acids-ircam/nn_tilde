#include "../shared/nn_base.h"
#include "c74_min.h"


long simplemc_multichanneloutputs(c74::max::t_object *x, long index,
                                  long count);
long simplemc_inputchanged(c74::max::t_object *x, long index, long count);


class mcs_nn: public nn_base<mcs_nn, mc_operator<>> {

public:
    MIN_DESCRIPTION{"Multi-channel interface for deep learning models"};
    MIN_TAGS{"audio, deep learning, ai"};
    MIN_AUTHOR{"Antoine Caillon & Axel Chemla--Romeu-Santos"};
    MIN_RELATED{"nn.info, nn~, mc.nn~"};

    static std::string get_external_name() { return "mcs.nn~";} 
    mcs_nn(const atoms &args = {}) {
        init_external(args); 
    }

    int get_sample_rate() override {
        return samplerate(); 
    }

    void perform(DspState &s, audio_bundle input, audio_bundle output) override;
    std::unique_ptr<DspState> build_state() override;

    void dump_object() override;

    // channel handling
    int get_batches(); 
    int n_mc_inputs() {
      return std::accumulate(channel_map.begin(), channel_map.end(), 0);
    } 
    bool check_inputs(); 
    std::vector<long> channel_map;

    void init_inputs_and_outputs(const atoms& atoms) override; 
    void init_inlets_and_outlets() override; 
    void update_method(std::string method = "") override;
    bool update_channel_map(const long& index, const long& count); 

    int m_out_channels = 0; 
    int m_out_channels_arg = 0; 
    // returns true if the number of output channels changed
    bool update_out_channels() {
      if ((m_out_channels_arg == 0) && (m_model_out != m_out_channels)) {
        DEBUG_PRINT("updating out channels to %d", m_model_out);
        m_out_channels = m_model_out; 
        return true;
      }
      return false;
      // if method is changed, multi channels outputs will stay the same. 
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
              m_out_channels_arg = in_chans;
              m_out_channels = in_chans;
              DEBUG_PRINT("setting out channels to %d", in_chans);
              if (m_is_backend_init)
                request_rebuild();
            }
            return args;
          }
        }};
};

bool mcs_nn::check_inputs() {
    return true;
}

int mcs_nn::get_batches() {
  return *std::max_element(channel_map.begin(), channel_map.end());
}

void mcs_nn::init_inputs_and_outputs(const atoms &args) {
  bool empty_mode = false; 
  DEBUG_PRINT("parsing inputs & outputs...");
  if (args.size() > 0) { // ONE ARGUMENT IS GIVEN
    auto model_path = std::string(args[0]);
    if (model_path == "void") {
      empty_mode = true;    
      m_model_in = 1; 
      m_model_out = 1; 
    } else {
      try {
        m_path = to_model_path(model_path);
      } catch (std::string &e) {
        error(e);
      }
    }
  }
  
  if (empty_mode) {
    DEBUG_PRINT("empty mode");
    if (args.size() > 1) { // FOUR ARGUMENTS ARE GIVEN
      n_batches = int(args[1]);
    }
    if (args.size() > 2) { // THREE ARGUMENTS ARE GIVEN
      m_buffer_size = int(args[2]);
    }
    channel_map = std::vector<long>(n_batches, 1); 
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
    DEBUG_PRINT("loading model..."); 
    load_model(m_path);
    if (m_ready) {
      m_out_channels = m_model_out; 
    } 
  }
  if (m_buffer_size == -1) {
    // NO THREAD MODE
    m_buffer_size = DEFAULT_BUFFER_SIZE;
  }
}

// returns true if the channel map changed (state needs to be rebuilt)
bool mcs_nn::update_channel_map(const long& index, const long& count)
{
  if (index < 0 || index >= (long)channel_map.size())
    return false;
  if (channel_map[index] != count) {
    channel_map[index] = count;
    return true;
  }
  return false; 
}


void mcs_nn::init_inlets_and_outlets() {
   
  DEBUG_PRINT("loading model..."); 
  DEBUG_PRINT("n_batches : %d", n_batches); 
  std::string input_label; 
  for (int i(0); i < n_batches; i++) {
    if (m_model_in > 0) {
      input_label = "(multichannel) batch " + std::to_string(i) + "(" + std::to_string(m_model_in) + " dimensions)";
    } else {
      input_label = "(multichannel) batch " + std::to_string(i); 
    }
    m_inlets.push_back(
      std::make_unique<inlet<>>(this, input_label, "multichannelsignal"));
  }

  std::string output_label;
  for (int i(0); i < n_batches; i++) {
    output_label = "(multichannel) batch " + std::to_string(i) + "(" + std::to_string(m_out_channels) + " dimensions)";
    m_outlets.push_back(
        std::make_unique<outlet<>>(this, output_label, "multichannelsignal"));
  }
  n_inlets = n_batches; 
  n_outlets = n_batches; 
}

std::unique_ptr<DspState> mcs_nn::build_state() {
  auto state = std::make_unique<DspState>();
  if (!m_model || !m_model->is_loaded())
    return state;
  update_method(); 
  if (!m_ready)
    return state;
  auto params = m_model->get_method_params(m_method);
  if (params.size() < 4)
    return state;
  if (m_out_channels == 0) {
    cerr << "could not retrieve number of output channels" << endl;
    return state;
  }

  state->method = m_method;
  state->model_in = params[0];
  state->model_out = params[2];
  state->buffer_size = effective_buffer_size();
  state->n_batches = n_inlets;
  state->n_out_channels = state->model_out;
  state->out_channels = m_out_channels;
  state->channel_map = channel_map;
  DEBUG_PRINT("building state: %d mc inputs, %d x %d outputs, buffer %d",
              n_mc_inputs(), n_outlets, m_out_channels, state->buffer_size);
  state->allocate(n_mc_inputs(), n_outlets * m_out_channels,
                  state->model_in * n_inlets, state->model_out * n_outlets);
  state->valid = true;
  return state;
}

void mcs_nn::update_method(std::string method) {
  if (!method.empty()) {
      set_method(method);
  }
  if (!m_model->is_loaded()) {
    cerr << "no model is set yet" << endl;
    return; 
  }
  if (m_model->has_method(m_method)) {
    auto params = m_model->get_method_params(m_method);
    // input parameters
    m_model_in = params[0];
    m_in_ratio = params[1];
    // output parameters
    m_model_out = params[2];
    m_out_ratio = params[3]; 

    if (m_out_channels == 0) { 
      m_out_channels = m_model_out; 
    } 
    m_ready = true;
  } else {
    cerr << "method " << method << " not present in model" << endl;
    m_ready = false; 
  }
}

void mcs_nn::perform(DspState &s, audio_bundle input, audio_bundle output) {
  auto chan_size = input.channel_count(); 
  auto vec_size = input.frame_count();
  const int n_map = s.channel_map.size();

  // COPY INPUT TO CIRCULAR BUFFER
  const int n_channels_in = std::min<int>(chan_size, s.buffer_in);
  for (int in_c(0); in_c < n_channels_in; in_c++) {
    s.in_buffer[in_c].put(input.samples(in_c), vec_size);
  }

  if (s.buffer_in > 0 && s.in_buffer[0].full()) { // BUFFER IS FULL

    // input channel i (flat, inlet by inlet) -> model buffer chan * n_batches + batch
    auto transfer_in = [&]() {
      int current_chan = 0; 
      int current_batch = 0; 
      int i = 0;
      while (i < n_channels_in && current_batch < n_map) {
        if (current_chan >= s.model_in) {
          // extra channels in this batch : skip them
          i += (s.channel_map[current_batch] - current_chan); 
          current_batch += 1; 
          current_chan = 0; 
        } else { 
          auto c_idx = current_chan * s.n_batches + current_batch; 
          if (c_idx < (int)s.in_model.size())
            s.in_buffer[i].get(s.in_model[c_idx].get(), s.buffer_size);
          current_chan++; 
          if (current_chan >= s.channel_map[current_batch]) {
            current_batch += 1;
            current_chan = 0;
          }
          i++; 
        }
      }
    };

    // model buffer batch * model_out + chan -> output batch * out_channels + chan
    auto transfer_out = [&]() {
      const int n_channels = std::min(s.model_out, s.out_channels);
      const int n_out_batches = s.out_channels > 0 ? s.buffer_out / s.out_channels : 0;
      for (int b(0); b < n_out_batches; b++) {
        for (int c(0); c < n_channels; c++) { 
          auto b_idx = b * s.out_channels + c;
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
  for (int i(0); i < output.channel_count(); i++) {
    auto out = output.samples(i); 
    if (i < s.buffer_out) {
      s.out_buffer[i].get(out, vec_size); 
    } else {
      std::fill(out, out + vec_size, 0.);
    }
  }
}

long simplemc_multichanneloutputs(c74::max::t_object *x, long index,
                                  long count) {
  minwrap<mcs_nn> *ob = (minwrap<mcs_nn> *)(x);
  return ob->m_min_object.m_out_channels;
}

long simplemc_inputchanged(c74::max::t_object *x, long index, long count) {
  minwrap<mcs_nn> *ob = (minwrap<mcs_nn> *)(x);
  auto &self = ob->m_min_object;
  bool map_changed = self.update_channel_map(index, count);
  bool out_changed = self.update_out_channels();
  if (map_changed || out_changed)
    self.request_rebuild();

  if (count != self.m_model_in) {
    auto n_channels = ob->m_min_object.m_model_in; 
    c74::max::object_warn(
        x, (std::string("got " + std::to_string(count) + 
            " for " + std::to_string(n_channels) + 
            " model inputs").c_str())
    );
  }
  return true;
}

void mcs_nn::dump_object() {
  cout << "model_path: " << std::string(m_path) << endl;
  if (m_model) {
    if (m_model->is_loaded()) {
      cout << "input dimension: " << m_model_in << endl;
      cout << "output dimension: " << m_model_out << endl;
    } else {
      cout << "input dimension: no model yet" << endl;
      cout << "output dimension: no model yet" << endl;
    }
  } else {
    cout << "input dimension: no model yet" << endl;
    cout << "output dimension: no model yet" << endl;
  }
  cout << "input ratio: " << std::to_string(m_in_ratio) << endl; 
  cout << "output ratio: " << std::to_string(m_out_ratio) << endl; 
  cout << "methods: ";
  for (auto method: m_model->get_available_methods())
    cout << method << "; ";
  cout << endl; 
  cout << "attributes: ";
  for (auto attribute: m_model->get_settable_attributes())
    cout << attribute << "; ";
  cout << endl;
}

MIN_EXTERNAL(mcs_nn);