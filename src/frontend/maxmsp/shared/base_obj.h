
#include <chrono>
#include <semaphore>
#include <string>
#include <thread>
#include <vector>
#include "c74_min.h"
#include "max_model_download.h"
#include "buffer_tools.h"
#include "dict_utils.h"
#include "../../../backend/backend.h"
#include "../../../shared/circular_buffer.h"
#include "../../../shared/static_buffer.h"

#ifndef VERSION
#define VERSION "UNDEFINED"
#endif

#ifndef REFRESH_THREAD_INTERVAL
#define REFRESH_THREAD_INTERVAL 100
#endif

#ifndef DEFAULT_BUFFER_SIZE
#define DEFAULT_BUFFER_SIZE 4096
#endif

using namespace c74::min;

unsigned power_ceil(unsigned x) {
  if (x <= 1)
    return 1;
  int power = 2;
  x--;
  while (x >>= 1)
    power <<= 1;
  return power;
}

template <typename nn_name> 
class nn_base : public object<nn_name>, public vector_operator<> {
public:
  
  using MaxBufferList = std::map<std::string, StaticBuffer<float>>;

  nn_base(const atoms &args = {});
  ~nn_base<nn_name>();

  // INLETS OUTLETS
  std::vector<std::unique_ptr<inlet<>>> m_inlets;
  std::vector<std::unique_ptr<outlet<>>> m_outlets;

  // BACKEND RELATED MEMBERS
  std::unique_ptr<Backend> m_model;
  bool m_is_backend_init = false;
  bool m_ready = false;
  std::string m_method;
  std::vector<std::string> settable_attributes;
  bool has_settable_attribute(std::string attribute);
  c74::min::path m_path;
  int n_inlets, m_model_in, m_in_ratio, n_outlets, m_model_out, m_out_ratio, m_higher_ratio;
  bool is_valid_print_key(std::string string);
  void load_model(const std::string &model);
  void update_model(const std::string &model);
  path to_model_path(std::string model_path);

  // BUFFER ATTRIBUTES MANAGER
  BufferManager m_buffer_manager;

  // BUFFER RELATED MEMBERS
  int m_buffer_size;
  std::unique_ptr<circular_buffer<double, float>[]> m_in_buffer;
  std::unique_ptr<circular_buffer<float, double>[]> m_out_buffer;
  std::vector<std::unique_ptr<float[]>> m_in_model, m_out_model;

  // AUDIO PERFORM
  bool m_force_refresh, m_use_thread, m_should_stop_perform_thread;
  std::binary_semaphore m_data_available_lock, m_result_available_lock;
  std::unique_ptr<std::thread> m_compute_thread;

  void operator()(audio_bundle input, audio_bundle output);
  void perform(audio_bundle input, audio_bundle output);

  // FLUX
  logger cout;
  logger cerr;
  logger cwarn;

  // HELPERS
  void dump_object();
  void print_to_cout(std::string &message);
  void print_to_cerr(std::string &message);

  // DOWNLOAD RELATED ATTRIBUTES
  std::unique_ptr<MaxModelDownloader> m_downloader;
  void dump_available_models(); 

  // ONLY FOR DOCUMENTATION
  argument<symbol> path_arg{this, "model path",
                            "Absolute path to the pretrained model."};
  argument<symbol> method_arg{this, "method",
                              "Name of the method to call during synthesis."};
  argument<int> buffer_arg{
      this, "buffer size",
      "Size of the internal buffer (can't be lower than the method's ratio)."};

  // ENABLE / DISABLE ATTRIBUTE
  attribute<bool> enable{this, "enable", true,
                         description{"Enable / disable tensor computation"}};

  // ENABLE / DISABLE ATTRIBUTE
  attribute<bool> gpu{this, "gpu", false,
                      description{"Enable / disable gpu usage when available"},
                      setter{[this](const c74::min::atoms &args,
                                    const int inlet) -> c74::min::atoms {
                        if (m_is_backend_init)
                          m_model->use_gpu(bool(args[0]));
                        return args;
                      }}};

  // TRACK BUFFER ATTRIBUTE
  attribute<bool> track_buffers{this, 
                                "track_buffers", 
                                false, 
                                description{"tracks buffer change for buffer attributes"}, 
                                setter{
                                  MIN_FUNCTION{
                                    bool enable_buffer_tracking = args[0];
                                    if (m_is_backend_init) {
                                      this->m_buffer_manager.set_buffer_tracking(enable_buffer_tracking);
                                    }
                                    return args;
                                  }}};


  // BOOT STAMP
  message<> maxclass_setup{
      this, "maxclass_setup",
      [this](const c74::min::atoms &args, const int inlet) -> c74::min::atoms {
        cout << "nn_base~ " << VERSION << " - torch " << TORCH_VERSION
             << " - 2023 - Antoine Caillon & Axel Chemla--Romeu-Santos" << endl;
        cout << "visit https://forum.ircam.fr" << endl;
        return {};
      }};

  message<> print {
    this, "print", 
    MIN_FUNCTION {
      bool is_valid = is_valid_print_key(args[0]);
      if (!is_valid) {
        return {};
      }
      if (args[1] == "cout") {
        cout << args[2] << endl;
      } else if (args[1] == "cerr") {
        cerr << args[2] << endl;
      } else if (args[1] == "cwarn") {
        cwarn << args[2] << endl;
      }
      return {};
    }
  };

  message<> buffer_notify{
      this, "notify",
        MIN_FUNCTION {
          return buffer_reference::handle_notification<BufferManager::iterator>(
            this, 
            args, 
            m_buffer_manager.begin(), 
            m_buffer_manager.end()
          );
      }};

  message<> dump_callback{
      this, "dump", 
        description{"dumps model information to console"},
        MIN_FUNCTION {
          this->dump_object();  
          return {};
        }};

  message <> get_available_models_callback{
    this, "get_available_models", 
    description{"print available models to console"}, 
    MIN_FUNCTION {
        this->dump_available_models(); 
        return {};
    }};

  message <> download_models {
    this, "download", 
    description{"download a model from IRCAM Forum API"}, 
    MIN_FUNCTION {
      if (args.size() == 0) {
        cerr << "please provide a model card (print downloadable models with get_available_models messages)" << endl;
      }
      std::string model_card = args[0];
      try {
        if (this->m_downloader.get()->is_ready())
          this->m_downloader.get()->download(model_card);
      } catch (std::string &e) {
        cerr << e << endl;
      }
      return {}; 
    }
  };
  
  message <> delete_models {
    this, "delete", 
    description{"delete a model downloaded from IRCAM Forum API"}, 
    MIN_FUNCTION {
      if (args.size() == 0) {
        cerr << "please provide a model to delete" << endl;
      }
      std::string model_card = args[0];
      try {
        if (this->m_downloader.get()->is_ready())
          this->m_downloader.get()->remove(model_card);
      } catch (std::string &e) {
        cerr << e << endl;
      }
      return {}; 
  }};


  message<> anything{
      this, "anything", "callback for attributes",
      [this](const c74::min::atoms &args, const int inlet) -> c74::min::atoms {
        symbol attribute_name = args[0];
        if (attribute_name == "reload") {
          m_model->reload();
        } else if (attribute_name == "load") {
          if (args.size() < 2) {
            cerr << "a model path must be given along the model message" << endl; 
          } else {
            std::string model_path = args[1];
            update_model(model_path); 
          }
          return {}; 
        } else if (attribute_name == "get_attributes") {
          for (std::string attr : settable_attributes) {
            cout << attr << endl;
          }
          return {};
        } else if (attribute_name == "get_methods") {
          for (std::string method : m_model->get_available_methods())
            cout << method << endl;
          return {};
        } else if (attribute_name == "get") {
          if (args.size() < 2) {
            cerr << "get must be given an attribute name" << endl;
            return {};
          }
          attribute_name = args[1];
          if (m_model->has_settable_attribute(attribute_name)) {
            try {
              cout << attribute_name << ": "
                 << m_model->get_attribute_as_string(attribute_name) << endl;
            } catch (std::string& e) {
              cout << e << endl; 
            }
          } else {
            cerr << "no attribute " << attribute_name << " found in model"
                 << endl;
          }
          return {};
        } else if (attribute_name == "set") {
          if (args.size() < 3) {
            cerr << "set must be given an attribute name and corresponding "
                    "arguments"
                 << endl;
            return {};
          }
          attribute_name = args[1];
          std::vector<std::string> attribute_args;

          MaxBufferList buffers;
          if (has_settable_attribute(attribute_name)) {

            for (int i = 2; i < args.size(); i++) {
              // get if argument is buffer
              attribute_args.push_back(args[i]);
              m_buffer_manager.append_if_buffer_element(m_model.get(), buffers, args[i], attribute_name, i - 2);
            }
            try {
              m_model->set_attribute(attribute_name, attribute_args, buffers);
            } catch (std::string message) {
              cerr << message << endl;
            }
          } else {
            cerr << "model does not have attribute " << attribute_name << endl;
          }
        } else {
          cerr << "no corresponding method for " << attribute_name << endl;
        }
        return {};
      }};
};


template <typename nn_name>
void model_perform(nn_base<nn_name> *nn_base_instance) {
  std::vector<float *> in_model, out_model;
  for (int c(0); c < nn_base_instance->n_inlets; c++)
    in_model.push_back(nn_base_instance->m_in_model[c].get());
  for (int c(0); c < nn_base_instance->n_outlets; c++)
    out_model.push_back(nn_base_instance->m_out_model[c].get());

  nn_base_instance->m_model->perform(in_model, out_model, nn_base_instance->m_buffer_size,
                                nn_base_instance->m_method, 1);
}
template <typename nn_name>
void model_perform_loop(nn_base<nn_name> *nn_base_instance) {
  while (!nn_base_instance->m_ready){
    std::this_thread::sleep_for(std::chrono::milliseconds(REFRESH_THREAD_INTERVAL));
    if (nn_base_instance->m_should_stop_perform_thread) {
      return;
    }
  }
  std::vector<float *> in_model, out_model;
  for (int c(0); c < nn_base_instance->n_inlets; c++)
    in_model.push_back(nn_base_instance->m_in_model[c].get());
  for (int c(0); c < nn_base_instance->n_outlets; c++)
    out_model.push_back(nn_base_instance->m_out_model[c].get());

  while (!nn_base_instance->m_should_stop_perform_thread) {
    if (nn_base_instance->m_data_available_lock.try_acquire_for(
            std::chrono::milliseconds(REFRESH_THREAD_INTERVAL))) {
        nn_base_instance->m_model->perform(in_model, out_model,
                                      nn_base_instance->m_buffer_size,
                                      nn_base_instance->m_method, 1);
        nn_base_instance->m_result_available_lock.release();
    }
  }
}


template <typename nn_name>
bool nn_base<nn_name>::is_valid_print_key(std::string id_string) {
  if (id_string == m_buffer_manager.string_id()) {
    return true;
  } else if (id_string == m_downloader.get()->string_id()) {
    return true;
  }
  return false;
}

template <typename nn_name>
nn_base<nn_name>::nn_base(const atoms &args)
    : m_compute_thread(nullptr), n_inlets(-1), m_in_ratio(1), n_outlets(-1),
      m_out_ratio(1), m_buffer_size(-1), m_method("forward"), m_force_refresh(false),
      m_use_thread(true), m_data_available_lock(0), m_result_available_lock(1),
      m_should_stop_perform_thread(false), m_in_model(), m_out_model(), 
      cout(this, logger::type::message), cwarn(this, logger::type::warning), cerr(this, logger::type::error) {

  m_model = std::make_unique<Backend>();
  m_downloader = std::make_unique<MaxModelDownloader>(this, std::string("nn~")); 
  m_is_backend_init = true;

  // CHECK ARGUMENTS
  if (!args.size()) {
    return;
  }
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
      auto inlets_arg = int(args[1]);
      if (inlets_arg > 1) { n_inlets = inlets_arg; }
    }
    if (args.size() > 2) { // FIVE ARGUMENTS ARE GIVEN
      auto outlets_arg = int(args[2]);
      if (outlets_arg > 1) { n_outlets = outlets_arg; }
    } 
    if (args.size() > 3) { // THREE ARGUMENTS ARE GIVEN
      m_buffer_size = int(args[3]);
    }
    if (n_outlets == -1) { n_outlets = 1;}
  } else {
    if (args.size() > 1) { // TWO ARGUMENTS ARE GIVEN
      m_method = std::string(args[1]);
    }
    if (args.size() > 2) { // THREE ARGUMENTS ARE GIVEN
      m_buffer_size = int(args[2]);
    }
    if (args.size() > 3) { // FOUR ARGUMENTS ARE GIVEN
      auto inlets_arg = int(args[3]);
      if (inlets_arg > 1) { n_inlets = inlets_arg; }
    }
    if (args.size() > 4) { // FIVE ARGUMENTS ARE GIVEN
      auto outlets_arg = int(args[4]);
      if (outlets_arg > 1) { n_outlets = outlets_arg; }
    }
    load_model(m_path);
  }

  // CREATE INLETS, OUTLETS and BUFFERS
  // m_in_buffer = std::make_unique<circular_buffer<double, float>[]>(n_inlets);
  for (int i(0); i < n_inlets; i++) {
    std::string input_label = "";
    try {
      input_label = m_model->get_model()
                        .attr(m_method + "_input_labels")
                        .toList()
                        .get(i)
                        .toStringRef();
    } catch (...) {
      input_label = "(signal) model input " + std::to_string(i);
    }
    m_inlets.push_back(std::make_unique<inlet<>>(this, input_label, "signal"));
    // m_in_buffer[i].initialize(m_buffer_size);
    // m_in_model.push_back(std::make_unique<float[]>(m_buffer_size));
  }


  // m_out_buffer = std::make_unique<circular_buffer<float, double>[]>(n_outlets);
  for (int i(0); i < n_outlets; i++) {
    std::string output_label = "";
    try {
      output_label = m_model->get_model()
                         .attr(m_method + "_output_labels")
                         .toList()
                         .get(i)
                         .toStringRef();
    } catch (...) {
      output_label = "(signal) model output " + std::to_string(i);
    }
    m_outlets.push_back(
        std::make_unique<outlet<>>(this, output_label, "signal"));
    // m_out_buffer[i].initialize(m_buffer_size);
    // m_out_model.push_back(std::make_unique<float[]>(m_buffer_size));
  }

  if (m_buffer_size == 0) {
    m_use_thread = false;
  }
  // Calling forward in a thread causes memory leak in windows.
  // See https://github.com/pytorch/pytorch/issues/24237
  #ifdef _WIN32
  m_use_thread = false;
  #endif

  if (m_use_thread)
    m_compute_thread = std::make_unique<std::thread>(model_perform_loop<nn_name>, this);

}
template <typename nn_name>
nn_base<nn_name>::~nn_base() {
  m_should_stop_perform_thread = true;
  if (m_compute_thread)
    m_compute_thread->join();
}

template <typename nn_name>
path nn_base<nn_name>::to_model_path(std::string model_path) {
  try {
    if (model_path.substr(model_path.length() - 3) != ".ts")
      model_path = model_path + ".ts";
    auto parsed_path = path(model_path);
    return parsed_path;
  } catch (std::exception& e) {
    std::string exception_str = e.what();
    throw exception_str;
  }
}

template <typename nn_name>
void nn_base<nn_name>::update_model(const std::string &model) {
  load_model(model);
}

template <typename nn_name>
void nn_base<nn_name>::load_model(const std::string& model_path) {

  try {
    auto path = to_model_path(model_path);
    m_model.get()->load(path, samplerate(), &m_method);
  } catch (std::string &e) {
    cerr << e << endl;
  }
  m_model->use_gpu(gpu);
  m_higher_ratio = m_model->get_higher_ratio();
  m_buffer_manager.init_buffer_list(m_model.get(), this);

  // SET PARAMS
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

  if (m_model_in != n_inlets) {
    cwarn << "nn_base~ has been initialised with " << n_inlets << " inputs, but current model has " << m_model_in << endl;
  }
  if (m_model_out != n_outlets) {
    cwarn << "nn_base~ has been initialised with " << n_outlets << " outputs, but current model has " << m_model_out << endl;
  }

  // GET MODEL'S SETTABLE ATTRIBUTES
  try {
    settable_attributes = m_model->get_settable_attributes();
  } catch (...) {
    cwarn << "could not retrieve model attributes ; maybe because of deprecated model." << endl;
  }

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

  // Calling forward in a thread causes memory leak in windows.
  // See https://github.com/pytorch/pytorch/issues/24237
#ifdef _WIN32
  m_use_thread = false;
#endif
  // TO DO : buffer size change (or prevent this do be possible)
  if (m_in_buffer.get() == nullptr) {
    m_in_buffer = std::make_unique<circular_buffer<double, float>[]>(n_inlets);
    if (m_in_buffer.get()->max_size() < m_buffer_size) {
      for (int i = 0; i < n_inlets; i++) {
        m_in_buffer[i].initialize(m_buffer_size);
      }
    }
  } 

  if (m_out_buffer.get() == nullptr) {
    if (m_out_buffer.get() != nullptr) { m_out_buffer.release(); }
    m_out_buffer = std::make_unique<circular_buffer<float, double>[]>(n_outlets);
    if (m_out_buffer.get()->max_size() < m_buffer_size) {
      for (int i = 0; i < n_outlets; i++) {
        m_out_buffer[i].initialize(m_buffer_size);
      }
    }
  } 

  if (m_in_model.size() == 0) {
    for (int i = 0; i < n_inlets; i++) {
      m_in_model.push_back(std::make_unique<float[]>(m_buffer_size));
      auto buf = m_in_model[i].get();
      for (int j = 0; j < m_buffer_size; j++) {buf[j] = 0.; }
    }
  }

  if (m_out_model.size() == 0) {
    for (int i = 0; i < n_outlets; i++) {
      m_out_model.push_back(std::make_unique<float[]>(m_buffer_size));
      auto buf = m_out_model[i].get();
      for (int j = 0; j < m_buffer_size; j++) {buf[j] = 0.; }
    }
  }

  m_ready = true;
  return;
}

template <typename nn_name>
bool nn_base<nn_name>::has_settable_attribute(std::string attribute) {
  for (std::string candidate: settable_attributes) {
    if (candidate == attribute)
      return true;
  }
  return false;
}

template <typename nn_name>
void nn_base<nn_name>::operator()(audio_bundle input, audio_bundle output) {
  auto dsp_vec_size = output.frame_count();

  // CHECK IF MODEL IS LOADED AND ENABLED
  if (!m_model->is_loaded() || !enable) {
    fill_with_zero(output);
    return;
  }

  // CHECK IF DSP_VEC_SIZE IS LARGER THAN BUFFER SIZE
  if (dsp_vec_size > m_buffer_size) {
    cerr << "vector size (" << dsp_vec_size << ") ";
    cerr << "larger than buffer size (" << m_buffer_size << "). ";
    cerr << "disabling model.";
    cerr << endl;
    enable = false;
    fill_with_zero(output);
    return;
  }
  perform(input, output);
}

template <typename nn_name>
void nn_base<nn_name>::perform(audio_bundle input, audio_bundle output) {
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
          model_perform<nn_name>(this);

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

template <typename nn_name>
void nn_base<nn_name>::dump_object() {
  cout << "model_path: " << std::string(m_path) << endl;
  cout << "inlets: " << std::to_string(m_inlets.size()) << endl;
  cout << "outlets: " << std::to_string(m_outlets.size()) << endl;
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
template <typename nn_name>
void nn_base<nn_name>::print_to_cout(std::string &message) {
  cout << message << endl;
}

template <typename nn_name>
void nn_base<nn_name>::print_to_cerr(std::string &message) {
  cerr << message << endl;
}

template <typename nn_name>
void nn_base<nn_name>::dump_available_models() {
  try {
    if (m_downloader.get()->is_ready()) {
      m_downloader.get()->print_available_models();
    }
  } catch (std::string &e) {
    cerr << "error from model downloader : " << e << endl;
  }
}
