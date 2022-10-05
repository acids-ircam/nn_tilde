#include "../../../backend/backend.h"
#include "c74_min.h"
#include "../shared/circular_buffer.h"
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
  MIN_AUTHOR{"Antoine Caillon"};

  nn(const atoms &args = {});
  ~nn();

  // INLETS OUTLETS
  std::vector<std::unique_ptr<inlet<>>> m_inlets;
  std::vector<std::unique_ptr<outlet<>>> m_outlets;

  // BACKEND RELATED MEMBERS
  Backend m_model;
  std::string m_method;
  c74::min::path m_path;
  int m_in_dim, m_in_ratio, m_out_dim, m_out_ratio, m_higher_ratio;

  // BUFFER RELATED MEMBERS
  int m_buffer_size;
  std::unique_ptr<circular_buffer<double, float>[]> m_in_buffer;
  std::unique_ptr<circular_buffer<float, double>[]> m_out_buffer;
  std::vector<std::unique_ptr<float[]>> m_in_model, m_out_model;

  // AUDIO PERFORM
  bool m_use_thread;
  std::unique_ptr<std::thread> m_compute_thread;
  void operator()(audio_bundle input, audio_bundle output);
  void buffered_perform(audio_bundle input, audio_bundle output);
  void perform(audio_bundle input, audio_bundle output);

  using vector_operator::operator();

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

  // BOOT STAMP
  message<> maxclass_setup{
      this, "maxclass_setup",
      [this](const c74::min::atoms &args, const int inlet) -> c74::min::atoms {
        cout << "nn~ - " << VERSION << " - 2022 - Antoine Caillon" << endl;
        cout << "visit https://caillonantoine.github.io" << endl;
        return {};
      }}

  ;
};

void model_perform(nn *nn_instance) {
  std::vector<float *> in_model, out_model;

  for (int c(0); c < nn_instance->m_in_dim; c++)
    in_model.push_back(nn_instance->m_in_model[c].get());
  for (int c(0); c < nn_instance->m_out_dim; c++)
    out_model.push_back(nn_instance->m_out_model[c].get());

  nn_instance->m_model.perform(in_model, out_model, nn_instance->m_buffer_size,
                               nn_instance->m_method, 1);
}

nn::nn(const atoms &args)
    : m_compute_thread(nullptr), m_in_dim(1), m_in_ratio(1), m_out_dim(1),
      m_out_ratio(1), m_buffer_size(4096), m_method("forward"),
      m_use_thread(true) {

  m_model = Backend();

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
  if (m_model.load(std::string(m_path))) {
    cerr << "error during loading" << endl;
    error();
    return;
  }

  m_higher_ratio = m_model.get_higher_ratio();

  // GET MODEL'S METHOD PARAMETERS
  auto params = m_model.get_method_params(m_method);

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
    m_inlets.push_back(std::make_unique<inlet<>>(
        this, "(signal) model input " + std::to_string(i), "signal"));
    m_in_buffer[i].initialize(m_buffer_size);
    m_in_model.push_back(std::make_unique<float[]>(m_buffer_size));
  }

  m_out_buffer = std::make_unique<circular_buffer<float, double>[]>(m_out_dim);
  for (int i(0); i < m_out_dim; i++) {
    m_outlets.push_back(std::make_unique<outlet<>>(
        this, "(signal) model output " + std::to_string(i), "signal"));
    m_out_buffer[i].initialize(m_buffer_size);
    m_out_model.push_back(std::make_unique<float[]>(m_buffer_size));
  }
}

nn::~nn() {
  if (m_compute_thread)
    m_compute_thread->join();
}

void fill_with_zero(audio_bundle output) {
  for (int c(0); c < output.channel_count(); c++) {
    auto out = output.samples(c);
    for (int i(0); i < output.frame_count(); i++) {
      out[i] = 0.;
    }
  }
}

void nn::operator()(audio_bundle input, audio_bundle output) {
  auto dsp_vec_size = output.frame_count();

  // CHECK IF MODEL IS LOADED AND ENABLED
  if (!m_model.is_loaded() || !enable) {
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
    // IF USE THREAD, CHECK THAT COMPUTATION IS OVER
    if (m_compute_thread && m_use_thread) {
      m_compute_thread->join();
    }

    // TRANSFER MEMORY BETWEEN INPUT CIRCULAR BUFFER AND MODEL BUFFER
    for (int c(0); c < m_in_dim; c++)
      m_in_buffer[c].get(m_in_model[c].get(), m_buffer_size);

    if (!m_use_thread) // PROCESS DATA RIGHT NOW
      model_perform(this);

    // TRANSFER MEMORY BETWEEN OUTPUT CIRCULAR BUFFER AND MODEL BUFFER
    for (int c(0); c < m_out_dim; c++)
      m_out_buffer[c].put(m_out_model[c].get(), m_buffer_size);

    if (m_use_thread) // PROCESS DATA LATER
      m_compute_thread = std::make_unique<std::thread>(model_perform, this);
  }

  // COPY CIRCULAR BUFFER TO OUTPUT
  for (int c(0); c < output.channel_count(); c++) {
    auto out = output.samples(c);
    m_out_buffer[c].get(out, vec_size);
  }
}

MIN_EXTERNAL(nn);
