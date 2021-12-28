#include "../../../backend/backend.h"
#include "c74_min.h"
#include <string>
#include <thread>
#include <vector>

using namespace c74::min;

class nn : public object<nn>, public vector_operator<> {
public:
  MIN_DESCRIPTION{"Deep Audio Engine"};
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
  std::unique_ptr<std::thread> compute_thread;

  // BUFFER RELATED MEMBERS
  int m_head, m_in_dim, m_in_ratio, m_out_dim, m_out_ratio;
  std::vector<std::unique_ptr<float[]>> m_in_buffer, m_out_buffer;

  // AUDIO PERFORM
  void operator()(audio_bundle input, audio_bundle output);
  using vector_operator::operator();
  int m_buffer_size;

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
      MIN_FUNCTION{cout << "nn~ - 1.11 - 2021 - Antoine Caillon" << endl;
  cout << "visit https://caillonantoine.github.io" << endl;
  return {};
}
}

;
}
;

void thread_perform(nn *nn_instance, std::vector<float *> in_buffer,
                    std::vector<float *> out_buffer, int n_vec,
                    std::string method) {
  nn_instance->m_model.perform(in_buffer, out_buffer, n_vec, method);
}

nn::nn(const atoms &args)
    : m_head(0), compute_thread(nullptr), m_in_dim(1), m_in_ratio(1),
      m_out_dim(1), m_out_ratio(1), m_buffer_size(4096), m_method("forward") {

  // CHECK ARGUMENTS
  if (!args.size()) {
    return;
  }
  if (args.size() > 0) {
    m_path = path(std::string(args[0]));
  }
  if (args.size() > 1) {
    m_method = std::string(args[1]);
  }
  if (args.size() > 2) {
    m_buffer_size = int(args[2]);
  }

  // TRY TO LOAD MODEL
  if (m_model.load(std::string(m_path))) {
    cout << "error during loading" << endl;
    return;
  } else {
    // cout << "successfully loaded model" << endl;
  }

  // GET MODEL'S METHOD PARAMETERS
  auto params = m_model.get_method_params(m_method);

  if (!params.size()) {
    cout << "method " << m_method << " not found, using forward instead"
         << endl;
    m_method = "forward";
    params = m_model.get_method_params(m_method);
  }

  m_in_dim = params[0];
  m_in_ratio = params[1];
  m_out_dim = params[2];
  m_out_ratio = params[3];

  if (m_buffer_size < m_in_ratio || m_buffer_size < m_out_ratio) {
    m_buffer_size = std::max(m_in_ratio, m_out_ratio);
    cout << "buffer size too small, switching to " << m_buffer_size << endl;
  }

  // CREATE INLETS, OUTLETS and BUFFERS
  for (int i(0); i < m_in_dim; i++) {
    m_inlets.push_back(std::make_unique<inlet<>>(
        this, "(signal) model input " + std::to_string(i), "signal"));
    m_in_buffer.push_back(std::make_unique<float[]>(2 * m_buffer_size));
  }
  for (int i(0); i < m_out_dim; i++) {
    m_outlets.push_back(std::make_unique<outlet<>>(
        this, "(signal) model output " + std::to_string(i), "signal"));
    m_out_buffer.push_back(std::make_unique<float[]>(2 * m_buffer_size));
  }
}

nn::~nn() {
  if (compute_thread) {
    compute_thread->join();
  }
}

void nn::operator()(audio_bundle input, audio_bundle output) {
  if (!m_model.is_loaded() || !enable) {
    for (int c(0); c < output.channel_count(); c++) {
      auto out = output.samples(c);
      for (int i(0); i < output.frame_count(); i++) {
        out[i] = 0.;
      }
    }
    return;
  }

  // TRANSFER MEMORY FROM BUFFERS - MAY BE OPTIMIZED TO AVOID
  // NESTED LOOPS
  for (int c(0); c < input.channel_count(); c++) {
    auto in = input.samples(c);
    for (int i(0); i < input.frame_count(); i++) {
      m_in_buffer[c][i + m_head] = float(in[i]);
    }
  }
  for (int c(0); c < output.channel_count(); c++) {
    auto out = output.samples(c);
    for (int i(0); i < output.frame_count(); i++) {
      out[i] = double(m_out_buffer[c][i + m_head]);
    }
  }

  // INCREASE HEAD
  m_head += input.frame_count();

  // IF BUFFER FILLED
  if (!(m_head % m_buffer_size)) {
    if (compute_thread) {
      compute_thread->join();
    }

    m_head = m_head % (2 * m_buffer_size);

    int offset_head = (m_head + m_buffer_size) % (2 * m_buffer_size);

    std::vector<float *> in_buffer;
    std::vector<float *> out_buffer;

    for (int i(0); i < m_in_buffer.size(); i++) {
      in_buffer.push_back(&m_in_buffer[i][offset_head]);
    }
    for (int i(0); i < m_out_buffer.size(); i++) {
      out_buffer.push_back(&m_out_buffer[i][offset_head]);
    }

    compute_thread = std::make_unique<std::thread>(
        thread_perform, this, in_buffer, out_buffer, m_buffer_size, m_method);
  }
}

MIN_EXTERNAL(nn);
