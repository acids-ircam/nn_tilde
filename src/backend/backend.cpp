#include "backend.h"
#include <algorithm>
#include <iostream>
#include <stdlib.h>

#define CUDA torch::kCUDA
#define CPU torch::kCPU

std::mutex Backend::m_render;
bool Backend::init = true;

Backend::Backend()
    : m_loaded(0), 
    m_cuda_available(torch::cuda::is_available())
    //m_cuda_available(false) 
{
    at::init_num_threads();

    init = false;
    
    if (init) {
        std::cout << "disabling the interop thread pool" << std::endl;
        at::set_num_threads(1);           // Disables the intraop thread pool.
        at::set_num_interop_threads(1);  // Disables the interop thread pool.
        init = false;
    }
    

    std::cout << at::get_num_threads() << std::endl;
    std::cout << at::get_num_interop_threads() << std::endl;

  if (m_cuda_available)
    std::cout << "using cuda" << std::endl;
  else
    std::cout << "using cpu" << std::endl;
}

void Backend::perform(std::vector<float *> in_buffer,
                      std::vector<float *> out_buffer, int n_vec,
                      std::string method, int n_batches) {
  c10::InferenceMode guard;
  torch::NoGradGuard no_grad_guard;

  auto params = get_method_params(method);
  if (!params.size())
    return;

  auto in_dim = params[0];
  auto in_ratio = params[1];
  auto out_dim = params[2];
  auto out_ratio = params[3];

  if (!m_loaded)
    return;

  // COPY BUFFER INTO A TENSOR
  // std::cout << "copying buffer in tensor" << std::endl;
  std::vector<at::Tensor> tensor_in;
  for (int i(0); i < in_buffer.size(); i++) {
      at::Tensor in_buf = torch::from_blob(in_buffer[i], { 1, 1, n_vec });
      in_buf.set_requires_grad(false);
      tensor_in.push_back(in_buf);
  }
  auto cat_tensor_in = torch::cat(tensor_in, 1);
  cat_tensor_in = cat_tensor_in.reshape({n_batches, in_dim, -1, in_ratio});
  cat_tensor_in = cat_tensor_in.select(-1, -1);

  if (m_cuda_available)
    cat_tensor_in = cat_tensor_in.to(CUDA);

  std::vector<torch::jit::IValue> inputs = {cat_tensor_in};

  //torch::Tensor input_tensor = torch::rand({ n_batches, in_dim, n_vec });

  // PROCESS TENSOR
  // std::cout << "tensor shape = " << cat_tensor_in.sizes() << std::endl;
  // std::cout << "processing tensor" << std::endl;
  at::Tensor tensor_out;
  try {
    // don't need to lock if not on gpu
    if (m_cuda_available) lock();
    torch::NoGradGuard no_grad_guard;
    // tensor_out = m_model.forward(inputs).toTensor();
    /*
    if (method == "encode") {
        tensor_out = m_model.forward(inputs).toTensor();
    }
    else if (method == "decode") {
        tensor_out = m_model.decode(inputs).toTensor();
    }
    else {
        tensor_out = m_model.encode(inputs).toTensor();
    }
    */
    // tensor_out = m_model.get_method("forward")(inputs).toTensor();
    tensor_out = m_model.get_method(method)(inputs).toTensor();
    //tensor_out = torch::rand({ n_batches, out_dim, n_vec});
    if (m_cuda_available) unlock();
    //std::cout << tensor_out.size(0) << "," << tensor_out.size(1) << "," << tensor_out.size(2) << std::endl;
    // std::cout << out_dim << std::endl << std::endl;
    
    // tensor_out = m_model.get_method(method)(inputs).toTensor();
    //m_render.unlock();
        
    tensor_out = tensor_out.repeat_interleave(out_ratio).reshape(
        {n_batches, out_dim, -1});
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
    unlock();
    return;
  }

  int out_batches(tensor_out.size(0)), out_channels(tensor_out.size(1)),
      out_n_vec(tensor_out.size(2));

  // CHECKS ON TENSOR SHAPE
  if (out_batches * out_channels != out_buffer.size()) {
    std::cout << "bad out_buffer size, expected " << out_batches * out_channels
              << " buffers, got " << out_buffer.size() << "!\n";
    return;
  }

  if (out_n_vec != n_vec) {
    std::cout << "model output size is not consistent, expected " << n_vec
              << " samples, got " << out_n_vec << "!\n";
    return;
  }

  tensor_out = tensor_out.to(CPU);
  tensor_out = tensor_out.reshape({out_batches * out_channels, -1});
  auto out_ptr = tensor_out.contiguous().data_ptr<float>();

  for (int i(0); i < out_buffer.size(); i++) {
    memcpy(out_buffer[i], out_ptr + i * n_vec, n_vec * sizeof(float));
  }
}

int Backend::load(std::string path) {
  try {
    m_model = torch::jit::load(path);
    m_model.eval();
    if (m_cuda_available) {
      std::cout << "sending model to gpu" << std::endl;
      m_model.to(CUDA);
    }
    m_loaded = 1;
    m_filepath = path;
    return 0;
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
    return 1;
  }
}

std::vector<std::string> Backend::get_available_methods() {
  std::vector<std::string> methods;
  for (const auto &m : m_model.get_methods())
    methods.push_back(m.name());
  return methods;
}

std::vector<std::string> Backend::get_available_attributes() {
  std::vector<std::string> attributes;
  for (const auto &attribute : m_model.named_attributes())
    attributes.push_back(attribute.name);
  return attributes;
}

std::vector<int> Backend::get_method_params(std::string method) {
  auto am = get_available_methods();
  std::vector<int> params;

  if (std::find(am.begin(), am.end(), method) != am.end()) {
    try {
      auto p = m_model.attr(method + "_params").toTensor();
      for (int i(0); i < 4; i++)
        params.push_back(p[i].item().to<int>());
    } catch (...) {
    }
  }
  return params;
}

int Backend::get_higher_ratio() {
  int higher_ratio = 1;
  auto model_methods = get_available_methods();
  for (const auto &method : model_methods) {
    auto params = get_method_params(method);
    if (!params.size())
      continue; // METHOD NOT USABLE, SKIPPING
    int max_ratio = std::max(params[1], params[3]);
    higher_ratio = std::max(higher_ratio, max_ratio);
  }
  return higher_ratio;
}

bool Backend::is_loaded() { return m_loaded; }


// lock and unlock mutex - this only is done on windows when the model
// on the GPU. Due to some dumb shenanigans on the windows side, the models
// deadlock on the gpu when multithreaded. AFAIK this isn't a problem 
// on other platforms. This of course affects performance.
void Backend::lock() {
#ifdef _WIN32
    m_render.lock();
#endif
}

void Backend::unlock() {
#ifdef _WIN32
    m_render.unlock();
#endif
}
