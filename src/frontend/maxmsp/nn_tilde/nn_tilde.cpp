#include "../../../backend/backend.h"
#include "../../../shared/circular_buffer.h"
#include "../../../shared/static_buffer.h"
#include "../shared/buffer_tools.h"
#include "c74_min.h"
#include <chrono>
#include <semaphore>
#include <string>
#include <thread>
#include <vector>

#ifndef VERSION
#define VERSION "UNDEFINED"
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


class nn : public object<nn>, public vector_operator<> {
public:
  MIN_DESCRIPTION{"Interface for deep learning models"};
  MIN_TAGS{"audio, deep learning, ai"};
  MIN_AUTHOR{"Antoine Caillon & Axel Chemla--Romeu-Santos"};

  using MaxBufferList = std::map<std::string, StaticBuffer<float>>;

  nn(const atoms &args = {});
  ~nn();
  // int bind_buffer_attribute(std::string &element, buffer_reference* bufferRef);
  // int unbind_buffer_attribute(std::string &element, buffer_reference* bufferRef);
  // int modify_buffer_attribute(std::string &element, buffer_reference* bufferRef);
  // StaticBuffer<float> static_buffer_from_name(std::string buffer_name);
  // void link_attribute_to_buffer(std::string buffer_name, symbol target_max_buffer);

  // INLETS OUTLETS
  std::vector<std::unique_ptr<inlet<>>> m_inlets;
  std::vector<std::unique_ptr<outlet<>>> m_outlets;

  // BACKEND RELATED MEMBERS
  std::unique_ptr<Backend> m_model;
  bool m_is_backend_init = false;
  std::string m_method;
  std::vector<std::string> settable_attributes;
  bool has_settable_attribute(std::string attribute);
  c74::min::path m_path;
  int m_in_dim, m_in_ratio, m_out_dim, m_out_ratio, m_higher_ratio;
  bool is_valid_print_key(std::string string);

  // BUFFER ATTRIBUTES MANAGER
  BufferManager m_buffer_manager;

  // BUFFER RELATED MEMBERS
  int m_buffer_size;
  std::unique_ptr<circular_buffer<double, float>[]> m_in_buffer;
  std::unique_ptr<circular_buffer<float, double>[]> m_out_buffer;
  std::vector<std::unique_ptr<float[]>> m_in_model, m_out_model;

  // AUDIO PERFORM
  bool m_use_thread, m_should_stop_perform_thread;
  std::unique_ptr<std::thread> m_compute_thread;
  std::binary_semaphore m_data_available_lock, m_result_available_lock;

  void operator()(audio_bundle input, audio_bundle output);
  void perform(audio_bundle input, audio_bundle output);

  // HELPERS
  void dump_object();
  void print_to_cout(std::string &message);
  void print_to_cerr(std::string &message);

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
        cout << "nn~ " << VERSION << " - torch " << TORCH_VERSION
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

  message<> anything{
      this, "anything", "callback for attributes",
      [this](const c74::min::atoms &args, const int inlet) -> c74::min::atoms {
        symbol attribute_name = args[0];
        if (attribute_name == "reload") {
          m_model->reload();
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

void model_perform(nn *nn_instance) {
  std::vector<float *> in_model, out_model;
  for (int c(0); c < nn_instance->m_in_dim; c++)
    in_model.push_back(nn_instance->m_in_model[c].get());
  for (int c(0); c < nn_instance->m_out_dim; c++)
    out_model.push_back(nn_instance->m_out_model[c].get());

  nn_instance->m_model->perform(in_model, out_model, nn_instance->m_buffer_size,
                                nn_instance->m_method, 1);
}

void model_perform_loop(nn *nn_instance) {
  std::vector<float *> in_model, out_model;

  for (auto &ptr : nn_instance->m_in_model)
    in_model.push_back(ptr.get());

  for (auto &ptr : nn_instance->m_out_model)
    out_model.push_back(ptr.get());

  while (!nn_instance->m_should_stop_perform_thread) {
    if (nn_instance->m_data_available_lock.try_acquire_for(
            std::chrono::milliseconds(200))) {
      nn_instance->m_model->perform(in_model, out_model,
                                    nn_instance->m_buffer_size,
                                    nn_instance->m_method, 1);
      nn_instance->m_result_available_lock.release();
    }
  }
}

bool nn::is_valid_print_key(std::string id_string) {
  if (id_string == m_buffer_manager.string_id()) {
    return true;
  }
  return false;
}

nn::nn(const atoms &args)
    : m_compute_thread(nullptr), m_in_dim(1), m_in_ratio(1), m_out_dim(1),
      m_out_ratio(1), m_buffer_size(4096), m_method("forward"),
      m_use_thread(true), m_data_available_lock(0), m_result_available_lock(1),
      m_should_stop_perform_thread(false) {

  m_model = std::make_unique<Backend>();
  m_is_backend_init = true;

  // CHECK ARGUMENTS
  if (!args.size()) {
    return;
  }
  if (args.size() > 0) { // ONE ARGUMENT IS GIVEN
    auto model_path = std::string(args[0]);
    if (model_path.substr(model_path.length() - 3) != ".ts")
      model_path = model_path + ".ts";
    m_path = path(model_path);
  }
  if (args.size() > 1) { // TWO ARGUMENTS ARE GIVEN
    m_method = std::string(args[1]);
  }
  if (args.size() > 2) { // THREE ARGUMENTS ARE GIVEN
    m_buffer_size = int(args[2]);
  }

  // TRY TO LOAD MODEL
  if (m_model->load(std::string(m_path), samplerate())) {
    cerr << "error during loading" << endl;
    error();
    return;
  }

  m_model->use_gpu(gpu);
  m_higher_ratio = m_model->get_higher_ratio();

  // INIT BUFFER ATTRIBUTS
  m_buffer_manager.init_buffer_list(m_model.get(), this);
  
  // GET MODEL'S METHOD PARAMETERS
  auto params = m_model->get_method_params(m_method);

  // GET MODEL'S SETTABLE ATTRIBUTES
  try {
    settable_attributes = m_model->get_settable_attributes();
  } catch (...) {
  }

  if (!params.size()) {
    error("method " + m_method + " not found !");
  }

  m_in_dim = params[0];
  m_in_ratio = params[1];
  m_out_dim = params[2];
  m_out_ratio = params[3];

  if (!m_buffer_size) {
    // NO THREAD MODE
    m_use_thread = false;
    m_buffer_size = m_higher_ratio;
  } else if (m_buffer_size < m_higher_ratio) {
    m_buffer_size = m_higher_ratio;
    cerr << "buffer size too small, switching to " << m_buffer_size << endl;
  } else {
    m_buffer_size = power_ceil(m_buffer_size);
  }

  // Calling forward in a thread causes memory leak in windows.
  // See https://github.com/pytorch/pytorch/issues/24237
#ifdef _WIN32
  m_use_thread = false;
#endif

  // CREATE INLETS, OUTLETS and BUFFERS
  m_in_buffer = std::make_unique<circular_buffer<double, float>[]>(m_in_dim);
  for (int i(0); i < m_in_dim; i++) {
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
    m_in_buffer[i].initialize(m_buffer_size);
    m_in_model.push_back(std::make_unique<float[]>(m_buffer_size));
  }


  m_out_buffer = std::make_unique<circular_buffer<float, double>[]>(m_out_dim);
  for (int i(0); i < m_out_dim; i++) {
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
    m_out_buffer[i].initialize(m_buffer_size);
    m_out_model.push_back(std::make_unique<float[]>(m_buffer_size));
  }

  if (m_use_thread)
    m_compute_thread = std::make_unique<std::thread>(model_perform_loop, this);
}

nn::~nn() {
  m_should_stop_perform_thread = true;
  if (m_compute_thread)
    m_compute_thread->join();
}

bool nn::has_settable_attribute(std::string attribute) {
  for (std::string candidate : settable_attributes) {
    if (candidate == attribute)
      return true;
  }
  return false;
}

void nn::operator()(audio_bundle input, audio_bundle output) {
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



void nn::perform(audio_bundle input, audio_bundle output) {
  auto vec_size = input.frame_count();

  // COPY INPUT TO CIRCULAR BUFFER
  for (int c(0); c < input.channel_count(); c++) {
    auto in = input.samples(c);
    m_in_buffer[c].put(in, vec_size);
  }

  if (m_in_buffer[0].full()) { // BUFFER IS FULL
    if (!m_use_thread) {
      // TRANSFER MEMORY BETWEEN INPUT CIRCULAR BUFFER AND MODEL BUFFER
      for (int c(0); c < m_in_dim; c++)
        m_in_buffer[c].get(m_in_model[c].get(), m_buffer_size);

      // CALL MODEL PERFORM IN CURRENT THREAD
      model_perform(this);

      // TRANSFER MEMORY BETWEEN OUTPUT CIRCULAR BUFFER AND MODEL BUFFER
      for (int c(0); c < m_out_dim; c++)
        m_out_buffer[c].put(m_out_model[c].get(), m_buffer_size);

    } else if (m_result_available_lock.try_acquire()) {
      // TRANSFER MEMORY BETWEEN INPUT CIRCULAR BUFFER AND MODEL BUFFER
      for (int c(0); c < m_in_dim; c++)
        m_in_buffer[c].get(m_in_model[c].get(), m_buffer_size);

      // TRANSFER MEMORY BETWEEN OUTPUT CIRCULAR BUFFER AND MODEL BUFFER
      for (int c(0); c < m_out_dim; c++)
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

void nn::dump_object() {
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

// void nn::dump_object() {
//   if (!m_has_dump) {
//     return;
//   }
//   auto dump_outlet = m_outlets[m_outlets.size()].get();
//   dump_outlet->send(symbol("model_path"), symbol(std::string(m_path)));
//   dump_outlet->send(symbol("n_inlets"), symbol((int)m_inlets.size()));
//   dump_outlet->send(symbol("n_outlets"), symbol((int)m_outlets.size()));
//   dump_outlet->send(symbol("input_ratio"), symbol((int)m_in_ratio));
//   dump_outlet->send(symbol("output_ratio"), symbol((int)m_out_ratio));
//   for (auto method: m_model->get_available_methods())
//     dump_outlet->send(symbol("method"), symbol(method));
//   for (auto attribute: m_model->get_settable_attributes())
//     dump_outlet->send(symbol("attribute"), symbol(attribute));
// }

void nn::print_to_cout(std::string &message) {
  cout << message << endl;
}
void nn::print_to_cerr(std::string &message) {
  cerr << message << endl;
}

MIN_EXTERNAL(nn);
