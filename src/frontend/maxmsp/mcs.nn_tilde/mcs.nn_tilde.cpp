#include "../../../backend/backend.h"
#include "../shared/circular_buffer.h"
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

long simplemc_multichanneloutputs(c74::max::t_object *x, long index,
                                  long count);
long simplemc_inputchanged(c74::max::t_object *x, long index, long count);

class mc_bnn_tilde : public object<mc_bnn_tilde>, public mc_operator<> {
public:
  MIN_DESCRIPTION{
      "Multi-channel interface for deep learning models (batch version)"};
  MIN_TAGS{"audio, deep learning, ai"};
  MIN_AUTHOR{"Antoine Caillon, Axel Chemla--Romeu-Santos"};

  mc_bnn_tilde(const atoms &args = {});
  ~mc_bnn_tilde();

  // INLETS OUTLETS
  std::vector<std::unique_ptr<inlet<>>> m_inlets;
  std::vector<std::unique_ptr<outlet<>>> m_outlets;

  // CHANNELS
  std::vector<int> input_chans;
  int get_batches();
  bool check_inputs();

  // BACKEND RELATED MEMBERS
  std::unique_ptr<Backend> m_model;
  std::string m_method;
  std::vector<std::string> settable_attributes;
  bool has_settable_attribute(std::string attribute);
  c74::min::path m_path;
  int m_in_dim, m_in_ratio, m_out_dim, m_out_ratio, m_higher_ratio, m_batches;

  // BUFFER RELATED MEMBERS
  int m_buffer_size;
  std::unique_ptr<circular_buffer<double, float>[]> m_in_buffer;
  std::unique_ptr<circular_buffer<float, double>[]> m_out_buffer;
  std::vector<std::unique_ptr<float[]>> m_in_model, m_out_model;
  // void reset_buffers();

  // AUDIO PERFORM
  bool m_use_thread, m_should_stop_perform_thread;
  std::unique_ptr<std::thread> m_compute_thread;
  std::binary_semaphore m_data_available_lock, m_result_available_lock;

  void operator()(audio_bundle input, audio_bundle output);
  void perform(audio_bundle input, audio_bundle output);

  // using mc_operator::operator();

  // ONLY FOR DOCUMENTATION
  argument<symbol> path_arg{this, "model path",
                            "Absolute path to the pretrained model."};
  argument<symbol> method_arg{this, "method",
                              "Name of the method to call during synthesis."};
  argument<int> batches_arg{this, "batches", "Number of batches"};

  argument<int> buffer_arg{
      this, "buffer size",
      "Size of the internal buffer (can't be lower than the method's ratio)."};

  // ENABLE / DISABLE ATTRIBUTE
  attribute<bool> enable{this, "enable", true,
                         description{"Enable / disable tensor computation"}};

  // BOOT STAMP
  message<> maxclass_setup{
      this, "maxclass_setup",
      [this](const c74::min::atoms &args, const int inlet) -> c74::min::atoms {
        // make stamp
        cout << "nn~ " << VERSION << " - torch " << TORCH_VERSION
             << " - 2023 - Antoine Caillon & Axel Chemla--Romeu-Santos" << endl;
        cout << "visit https://caillonantoine.github.io" << endl;
        // mc handle
        c74::max::t_class *c = args[0];
        c74::max::class_addmethod(
            c, (c74::max::method)simplemc_multichanneloutputs,
            "multichanneloutputs", c74::max::A_CANT, 0);
        c74::max::class_addmethod(c, (c74::max::method)simplemc_inputchanged,
                                  "inputchanged", c74::max::A_CANT, 0);
        return {};
      }};

  message<> anything{this, "anything", "callback for attributes",
                     MIN_FUNCTION{symbol attribute_name = args[0];
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
      cout << attribute_name << ": "
           << m_model->get_attribute_as_string(attribute_name) << endl;
    } else {
      cerr << "no attribute " << attribute_name << " found in model" << endl;
    }
    return {};
  } else if (attribute_name == "set") {
    if (args.size() < 3) {
      cerr << "set must be given an attribute name and corresponding arguments"
           << endl;
      return {};
    }
    attribute_name = args[1];
    std::vector<std::string> attribute_args;
    if (has_settable_attribute(attribute_name)) {
      for (int i = 2; i < args.size(); i++) {
        attribute_args.push_back(args[i]);
      }
      try {
        m_model->set_attribute(attribute_name, attribute_args);
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
}
}
;
}
;

int mc_bnn_tilde::get_batches() { return m_batches; }

void model_perform(mc_bnn_tilde *mc_nn_instance) {
  std::vector<float *> in_model, out_model;
  auto num_batches = mc_nn_instance->get_batches();
  for (int c(0); c < mc_nn_instance->m_in_dim * num_batches; c++)
    in_model.push_back(mc_nn_instance->m_in_model[c].get());
  for (int c(0); c < mc_nn_instance->m_out_dim * num_batches; c++)
    out_model.push_back(mc_nn_instance->m_out_model[c].get());

  mc_nn_instance->m_model->perform(
      in_model, out_model, mc_nn_instance->m_buffer_size,
      mc_nn_instance->m_method, mc_nn_instance->get_batches());
}

void model_perform_loop(mc_bnn_tilde *mc_nn_instance) {
  std::vector<float *> in_model, out_model;

  for (auto &ptr : mc_nn_instance->m_in_model)
    in_model.push_back(ptr.get());

  for (auto &ptr : mc_nn_instance->m_out_model)
    out_model.push_back(ptr.get());

  while (!mc_nn_instance->m_should_stop_perform_thread) {
    if (mc_nn_instance->m_data_available_lock.try_acquire_for(
            std::chrono::milliseconds(200))) {
      mc_nn_instance->m_model->perform(
          in_model, out_model, mc_nn_instance->m_buffer_size,
          mc_nn_instance->m_method, mc_nn_instance->get_batches());
      mc_nn_instance->m_result_available_lock.release();
    }
  }
}

mc_bnn_tilde::mc_bnn_tilde(const atoms &args)
    : m_compute_thread(nullptr), m_in_dim(1), m_in_ratio(1), m_out_dim(1),
      m_out_ratio(1), m_buffer_size(4096), m_batches(1), m_method("forward"),
      m_use_thread(true), m_data_available_lock(0), m_result_available_lock(1),
      m_should_stop_perform_thread(false) {

  m_model = std::make_unique<Backend>();

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
    m_batches = int(args[2]);
  }
  if (args.size() > 3) { // FOUR ARGUMENTS ARE GIVEN
    m_buffer_size = int(args[3]);
  }

  // TRY TO LOAD MODEL
  if (m_model->load(std::string(m_path))) {
    cerr << "error during loading" << endl;
    error();
    return;
  }

  // FIND MINIMUM BUFFER SIZE GIVEN MODEL RATIO
  m_higher_ratio = 1;
  auto model_methods = m_model->get_available_methods();
  for (int i(0); i < model_methods.size(); i++) {
    auto params = m_model->get_method_params(model_methods[i]);
    if (!params.size())
      continue; // METHOD NOT USABLE, SKIPPING
    int max_ratio = std::max(params[1], params[3]);
    m_higher_ratio = std::max(m_higher_ratio, max_ratio);
  }

  // GET MODEL'S METHOD PARAMETERS
  auto params = m_model->get_method_params(m_method);

  if (!params.size()) {
    error("method " + m_method + " not found !");
  }

  // GET MODEL'S SETTABLE ATTRIBUTES
  try {
    settable_attributes = m_model->get_settable_attributes();
  } catch (...) {
  }

  m_in_dim = params[0];
  m_in_ratio = params[1];
  m_out_dim = params[2];
  m_out_ratio = params[3];
  for (int i(0); i < m_batches; i++)
    input_chans.push_back(1);

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

  // CREATE INLETS, OUTLETS
  for (int i(0); i < get_batches(); i++) {
    std::string input_label, output_label;
    try {
      input_label = m_model->get_model()
                        .attr(m_method + "_input_labels")
                        .toList()
                        .get(i)
                        .toStringRef();
    } catch (...) {
      input_label = "(signal) model input " + std::to_string(i);
    }
    try {
      output_label = m_model->get_model()
                         .attr(m_method + "_output_labels")
                         .toList()
                         .get(i)
                         .toStringRef();
    } catch (...) {
      output_label = "(signal) model output " + std::to_string(i);
    }
    m_inlets.push_back(
        std::make_unique<inlet<>>(this, input_label, "multichannelsignal"));
    m_outlets.push_back(
        std::make_unique<outlet<>>(this, output_label, "multichannelsignal"));
  }

  // CREATE BUFFERS
  m_in_buffer = std::make_unique<circular_buffer<double, float>[]>(
      m_in_dim * get_batches());
  for (int i(0); i < m_in_dim * get_batches(); i++) {
    m_in_buffer[i].initialize(m_buffer_size);
    m_in_model.push_back(std::make_unique<float[]>(m_buffer_size));
  }

  m_out_buffer = std::make_unique<circular_buffer<float, double>[]>(
      m_out_dim * get_batches());
  for (int i(0); i < m_out_dim * get_batches(); i++) {
    m_out_buffer[i].initialize(m_buffer_size);
    m_out_model.push_back(std::make_unique<float[]>(m_buffer_size));
  }

  if (m_use_thread)
    m_compute_thread = std::make_unique<std::thread>(model_perform_loop, this);
}

mc_bnn_tilde::~mc_bnn_tilde() {
  m_should_stop_perform_thread = true;
  if (m_compute_thread)
    m_compute_thread->join();
}

bool mc_bnn_tilde::has_settable_attribute(std::string attribute) {
  for (std::string candidate : settable_attributes) {
    if (candidate == attribute)
      return true;
  }
  return false;
}

void fill_with_zero(audio_bundle output) {
  for (int c(0); c < output.channel_count(); c++) {
    auto out = output.samples(c);
    for (int i(0); i < output.frame_count(); i++) {
      out[i] = 0.;
    }
  }
}

bool mc_bnn_tilde::check_inputs() {
  bool check = true;
  for (int i = 0; i < input_chans.size(); i++) {
    if (input_chans[i] != m_in_dim)
      check = false;
  }
  return check;
}

void mc_bnn_tilde::operator()(audio_bundle input, audio_bundle output) {
  auto dsp_vec_size = output.frame_count();

  // CHECK IF MODEL IS LOADED AND ENABLED
  if (!m_model->is_loaded() || !enable || !check_inputs()) {
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

void mc_bnn_tilde::perform(audio_bundle input, audio_bundle output) {
  auto vec_size = input.frame_count();

  // COPY INPUT TO CIRCULAR BUFFER
  for (int b(0); b < m_inlets.size(); b++) {
    for (int d(0); d < m_in_dim; d++) {
      auto in = input.samples(b * m_in_dim + d);
      m_in_buffer[d * get_batches() + b].put(in, vec_size);
    }
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
  for (int b(0); b < m_outlets.size(); b++) {
    for (int d(0); d < m_out_dim; d++) {
      auto out = output.samples(b * m_out_dim + d);
      m_out_buffer[b * m_out_dim + d].get(out, vec_size);
    }
  }
}

long simplemc_multichanneloutputs(c74::max::t_object *x, long index,
                                  long count) {
  minwrap<mc_bnn_tilde> *ob = (minwrap<mc_bnn_tilde> *)(x);
  return ob->m_min_object.m_out_dim;
}

long simplemc_inputchanged(c74::max::t_object *x, long index, long count) {
  minwrap<mc_bnn_tilde> *ob = (minwrap<mc_bnn_tilde> *)(x);
  auto chan_number = ob->m_min_object.m_in_dim;
  ob->m_min_object.input_chans[index] = count;
  if (chan_number != count) {
    c74::max::object_error(
        x, (std::string("invalid channel number for input ") +
            std::to_string(index) + std::string("; should be ") +
            std::to_string(chan_number))
               .c_str());
  }
  return false;
}

MIN_EXTERNAL(mc_bnn_tilde);
