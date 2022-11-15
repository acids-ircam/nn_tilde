#include "backend.h"
#include "parsing_utils.h"
#include "logger.h"
#include <algorithm>
#include <iostream>
#include <stdlib.h>

#define CPU torch::kCPU
#define CUDA torch::kCUDA
#define MPS torch::kMPS

Backend::Backend(Logger *logger): m_loaded(0), m_cuda_available(torch::cuda::is_available()) {
  // Check device
  at::init_num_threads();
  this->logger = logger;
}

void Backend::perform(std::vector<float *> in_buffer,
                      std::vector<float *> out_buffer, int n_vec,
                      std::string method, int n_batches) {
  c10::InferenceMode guard;

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
  std::vector<at::Tensor> tensor_in;
  for (int i(0); i < in_buffer.size(); i++) {
    tensor_in.push_back(torch::from_blob(in_buffer[i], {1, 1, n_vec}));
  }
  auto cat_tensor_in = torch::cat(tensor_in, 1);
  cat_tensor_in = cat_tensor_in.reshape({n_batches, in_dim, -1, in_ratio});
  cat_tensor_in = cat_tensor_in.select(-1, -1);
  if (this->device == "cuda")
    cat_tensor_in = cat_tensor_in.to(CUDA);
  if (this->device == "mps")
    cat_tensor_in = cat_tensor_in.to(MPS);

  std::vector<torch::jit::IValue> inputs = {cat_tensor_in};

  // PROCESS TENSOR
  at::Tensor tensor_out;
  try {
    tensor_out = m_model.get_method(method)(inputs).toTensor();
    tensor_out = tensor_out.repeat_interleave(out_ratio).reshape(
        {n_batches, out_dim, -1});
  } catch (const std::exception &e) {
    logger->error(e.what());
    std::cerr << e.what() << '\n';
    return;
  }
  int out_batches(tensor_out.size(0)), out_channels(tensor_out.size(1)),
      out_n_vec(tensor_out.size(2));

  // CHECKS ON TENSOR SHAPE
  if (out_batches * out_channels != out_buffer.size()) {
    logger->error("bad out_buffer size, expected " + std::to_string(out_batches * out_channels) + 
              " buffers, got " + std::to_string(out_buffer.size()));
    return;
  }

  if (out_n_vec != n_vec) {
    logger->error("model output size is not consistent, expected " + std::to_string(n_vec) + " samples, got " + std::to_string(out_n_vec));
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
  logger->post("loading model " + path + "...");
  try {
    m_model = torch::jit::load(path);
    model_path = path;
    m_model.eval();

    m_loaded = 1;
    return 0;
  } catch (const std::exception &e) {
    logger->error(e.what());
    std::cerr << e.what() << '\n';
    return 1;
  }
}

void Backend::reload() {
  load(model_path);
}

void Backend::set_device(std::string device) {
  if (device=="cuda") {
    if (!torch::cuda::is_available()) {
      logger->warning("CUDA not available. Set device to CPU.");
      this->device = "cpu";
    } else {
      this->device = "cuda";
    }
    if (m_loaded)
      m_model.to(CUDA);
  } else if (device=="mps") {
    // check if MPS is available?
    this->device = device;
    if (m_loaded)
      m_model.to(MPS);
  } else if (device == "cpu") {
    this->device = device;
    if (m_loaded)
      m_model.to(CPU);
  }
}

bool Backend::has_method(std::string method_name) {
  for (const auto &m : m_model.get_methods()) {
    if (m.name() == method_name)
      return true;
  }
  return false;
}

bool Backend::has_settable_attribute(std::string attribute) {
  for (const auto &a : get_settable_attributes()) {
    if (a == attribute)
      return true;
  }
  return false;
}

std::vector<std::string> Backend::get_available_methods() {
  std::vector<std::string> methods;
  try 
  {
    std::vector<c10::IValue> dumb_input = {};
    auto methods_from_model = m_model.get_method("get_methods")(dumb_input).toList();
    for (int i = 0; i < methods_from_model.size(); i++) {
      methods.push_back(methods_from_model.get(i).toStringRef());
    }
  } catch (...) {
    for (const auto &m : m_model.get_methods())
    {
      try 
      {
        auto method_params = m_model.attr(m.name() + "_params");
        methods.push_back(m.name());
      } catch (...) {}
    }
  }
  return methods;
}

std::vector<std::string> Backend::get_available_attributes() {
  std::vector<std::string> attributes;
  for (const auto &attribute : m_model.named_attributes())
    attributes.push_back(attribute.name);
  return attributes;
}

std::vector<std::string> Backend::get_settable_attributes() {
  std::vector<std::string> attributes;
  try 
  {
    std::vector<c10::IValue> dumb_input = {};
    auto methods_from_model = m_model.get_method("get_attributes")(dumb_input).toList();
    for (int i = 0; i < methods_from_model.size(); i++) {
      attributes.push_back(methods_from_model.get(i).toStringRef());
    }
  } catch (...) {
    for (const auto &a : m_model.named_attributes())
    {
      try 
      {
        auto method_params = m_model.attr(a.name + "_params");
        attributes.push_back(a.name);
      } catch (...) {}
    }
  }
  return attributes;
}

std::vector<c10::IValue> Backend::get_attribute(std::string attribute_name) {
  std::string attribute_getter_name = "get_" + attribute_name;
  try {
    auto attribute_getter = m_model.get_method(attribute_getter_name);
  } catch (...) {
    logger->error("getter for attribute " + attribute_name + " not found in model");
    throw "getter for attribute " + attribute_name + " not found in model";
  }
  std::vector<c10::IValue> getter_inputs = {}, attributes;
  try {
    try {
      attributes = m_model.get_method(attribute_getter_name)(getter_inputs).toList().vec();
    } catch (...) {
      auto output_tuple = m_model.get_method(attribute_getter_name)(getter_inputs).toTuple();
      attributes = (*output_tuple.get()).elements();
    }
  } catch(...) {
    attributes.push_back(m_model.get_method(attribute_getter_name)(getter_inputs));
  }
  return attributes;
}

std::string Backend::get_attribute_as_string(std::string attribute_name) {
  std::vector<c10::IValue> getter_outputs = get_attribute(attribute_name);
  // finstringd arguments
  torch::Tensor setter_params;
  try {
    setter_params = m_model.attr(attribute_name + "_params").toTensor();
  } catch (...) {
    logger->error("parameters to set attribute " + attribute_name + " not found in model");
    throw "parameters to set attribute " + attribute_name + " not found in model";
  } 
  std::string current_attr = "";
  for (int i = 0; i < setter_params.size(0); i++) {
    int current_id = setter_params[i].item().toInt();
    switch (current_id) {
      // bool case
      case 0: {
        current_attr += (getter_outputs[i].toBool())? "true" : "false";
        break;
      }
      // int case
      case 1: {
        current_attr += std::to_string(getter_outputs[i].toInt());
        break;
      }
      // float case
      case 2: {
        float result = getter_outputs[i].to<float>();
        current_attr += std::to_string(result);
        break;
      }
      // str case
      case 3: {
        current_attr += getter_outputs[i].toStringRef();
        break;
      }
      default: {
        logger->error("bad type id : " + std::to_string(current_id) + "at index " + std::to_string(i));
        throw "bad type id : " + std::to_string(current_id) + "at index " + std::to_string(i);
        break;
      }
    }
    if (i < setter_params.size(0) - 1)
      current_attr += " ";
  }
  return current_attr;
}

void Backend::set_attribute(std::string attribute_name, std::vector<std::string> attribute_args)
{
  // find setter
  std::string attribute_setter_name = "set_" + attribute_name;
  try {
    auto attribute_setter = m_model.get_method(attribute_setter_name);
  } catch (...) {
    logger->error("setter for attribute " + attribute_name + " not found in model");
    throw "setter for attribute " + attribute_name + " not found in model";
  }
  // find arguments
  torch::Tensor setter_params;
  try {
    setter_params = m_model.attr(attribute_name + "_params").toTensor();
  } catch (...) {
    logger->error("parameters to set attribute " + attribute_name + " not found in model");
    throw "parameters to set attribute " + attribute_name + " not found in model";
  }
  // process inputs
  std::vector<c10::IValue> setter_inputs = {};
  for (int i = 0; i < setter_params.size(0); i++) {
    int current_id = setter_params[i].item().toInt();
    switch (current_id) {
      // bool case
      case 0:
      setter_inputs.push_back(c10::IValue(to_bool(attribute_args[i])));
      break;
      // int case
      case 1:
      setter_inputs.push_back(c10::IValue(to_int(attribute_args[i])));
      break;
      // float case
      case 2:
      setter_inputs.push_back(c10::IValue(to_float(attribute_args[i])));
      break;
      // str case
      case 3:
      setter_inputs.push_back(c10::IValue(attribute_args[i]));
      break;
      default:
      logger->error("bad type id : " + std::to_string(current_id) + "at index " + std::to_string(i));
      throw "bad type id : " + std::to_string(current_id) + "at index " + std::to_string(i);
      break;
    }
  }
  try {
    auto setter_out = m_model.get_method(attribute_setter_name)(setter_inputs);
    int setter_result = setter_out.toInt();
    if (setter_result != 0) {
      logger->error("setter failed");
      throw "setter returned -1";
    }
  } catch (...) {
    logger->error("setter for " + attribute_name + " failed");
    throw "setter for " + attribute_name + " failed";
  }
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