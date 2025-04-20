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
  
  while (!nn_instance->can_perform()){
    std::this_thread::sleep_for(std::chrono::milliseconds(REFRESH_THREAD_INTERVAL));
    if (nn_instance->m_should_stop_perform_thread) {
      return;
    }
  }

  if (nn_instance->wait_for_buffer_reset) {
    nn_instance->init_buffers();
  }
  std::vector<float *> in_model, out_model;
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

    void init_external(const atoms &args) override {
        init_model(); 
        init_downloader(); 
        if (!args.size()) { return; } 
        init_inputs_and_outputs(args);
        init_inlets_and_outlets();
        // init_buffers(); 
        wait_for_buffer_reset = true; 
        init_process();
    } 
    void perform(audio_bundle input, audio_bundle output) override;

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
    void init_buffers() override; 
    void init_process() override; 
    void update_method(std::string method = "") override;
    bool update_channel_map(const long& index, const long& count); 

    int m_out_channels = 0; 
    int m_out_channels_arg = 0; 
    void update_out_channels() {
      if ((m_out_channels_arg == 0) && (m_model_out != m_out_channels)) {
        DEBUG_PRINT("updating out channels to %d", m_model_out);
        m_out_channels = m_model_out; 
        wait_for_buffer_reset = true; 
      }
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

void mcs_nn::init_buffers() {

  update_method(); 

  if (m_out_channels == 0) {
    error("could not retrieve number of output channels");
  }
  DEBUG_PRINT("initializing buffers...");
  if (!m_ready) { return; }
  if (m_buffer_size == -1) {
    // NO THREAD MODE
    m_buffer_size = DEFAULT_BUFFER_SIZE;
  }
  DEBUG_PRINT("buffer size : %d", m_buffer_size);
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

  m_buffer_in = n_mc_inputs(); 
  DEBUG_PRINT("initializing with n_mc_inputs : %d", m_buffer_in); 
  if (m_in_buffer.get() != nullptr) { m_in_buffer.release(); }
  m_in_buffer = std::make_unique<circular_buffer<double, float>[]>(m_buffer_in); 
  for (int i = 0; i < m_buffer_in; i++) {
    m_in_buffer[i].initialize(m_buffer_size);
  }

  DEBUG_PRINT("initializing with outputs : %d x %d", n_outlets, m_out_channels);
  if (m_out_buffer.get() != nullptr) { m_out_buffer.release(); }
  m_buffer_out = n_outlets * m_out_channels;
  m_out_buffer = std::make_unique<circular_buffer<float, double>[]>(m_buffer_out); 
  for (int i = 0; i < m_buffer_out; i++) {
    m_out_buffer[i].initialize(m_buffer_size);
  }

  DEBUG_PRINT("initializing with model buffer inputs : %d x %d", m_model_in, n_inlets);
  m_in_model.clear(); 
  for (int i = 0; i < m_model_in * n_inlets; i++) {
    m_in_model.push_back(std::make_unique<float[]>(m_buffer_size));
    std::fill(m_in_model[i].get(), m_in_model[i].get() + m_buffer_size, 0.); 
  }

  DEBUG_PRINT("initializing with model buffer outputs : %d x %d", m_model_out, n_outlets);
  m_out_model.clear(); 
  for (int i = 0; i < m_model_out * n_outlets; i++) {
    m_out_model.push_back(std::make_unique<float[]>(m_buffer_size));
    std::fill(m_out_model[i].get(), m_out_model[i].get() + m_buffer_size, 0.); 
  }
  DEBUG_PRINT("buffers initialized");
  wait_for_buffer_reset = false; 
  had_buffer_reset = true; 
  buffer_initialised = true; 
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
    wait_for_buffer_reset = true; 
  } else {
    cerr << "method " << method << " not present in model" << endl;
    m_ready = false; 
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

  // if (buffer_initialised) {
    int current_batch = 0; 
    int current_channel = 0; 
    int n_channels_in = std::min<int>(chan_size, m_buffer_in);
    for (int in_c(0); in_c < n_channels_in; in_c++) {
      auto in = input.samples(in_c);
      m_in_buffer[in_c].put(in, vec_size);
    }

    if (m_in_buffer[0].full()) { // BUFFER IS FULL
      if (!m_use_thread) {
        // TRANSFER MEMORY BETWEEN INPUT CIRCULAR BUFFER AND MODEL BUFFER
        int current_chan = 0; 
        int current_batch = 0; 
        for (int i(0); i < m_buffer_in; i++) {
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
        int i = 0;
        while (i < n_channels_in) {
          if (current_chan >= m_model_in) {
            i += (channel_map[current_batch] - current_chan); 
            current_batch += 1; 
            current_chan = 0; 
          } else { 
            if (current_chan < m_model_in) {
              auto c_idx = current_chan * n_batches + current_batch; 
              m_in_buffer[i].get(m_in_model[c_idx].get(), m_buffer_size);
            } 
            current_chan++; 
            if (current_chan >= channel_map[current_batch]) {
              current_batch += 1;
              current_chan = 0;
            }
            i++; 
          }
        }

        // TRANSFER MEMORY BETWEEN OUTPUT CIRCULAR BUFFER AND MODEL BUFFER
        auto current_idx = 0; 
        auto n_channels = std::min(m_model_out, m_out_channels);
        for (int b(0); b < n_outlets; b++) {
          for (int c(0); c < n_channels; c++){ 
              m_out_buffer[b * m_out_channels + c].put(m_out_model[b * m_model_out + c].get(), m_buffer_size);
          }
        }

        // SIGNAL PERFORM THREAD THAT DATA IS AVAILABLE
        m_data_available_lock.release();
      }
    }
    
    // for (int b(0); b < n_outlets; b++) {
    //   for (int c(0); c < n_channels; c++) {
    for (int i(0); i < m_buffer_out; i++) {
        auto out = output.samples(i); 
        m_out_buffer[i].get(out, vec_size); 
    }
  // }
}

long simplemc_multichanneloutputs(c74::max::t_object *x, long index,
                                  long count) {
  minwrap<mcs_nn> *ob = (minwrap<mcs_nn> *)(x);
  return ob->m_min_object.m_out_channels;
}

long simplemc_inputchanged(c74::max::t_object *x, long index, long count) {
  minwrap<mcs_nn> *ob = (minwrap<mcs_nn> *)(x);
  auto is_full = ob->m_min_object.update_channel_map(index, count);
  ob->m_min_object.update_out_channels();

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