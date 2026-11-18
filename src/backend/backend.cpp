#include "backend.h"
#include <exception>
#include "parsing_utils.h"
#include <algorithm>
#include <iostream>
#include <exception>
#include <stdio.h>
#include <stdlib.h>

#define CPU torch::kCPU
#define CUDA torch::kCUDA
#define MPS torch::kMPS


std::string tensor_to_str(torch::Tensor &tsr) {
  std::stringstream ss; 
  ss << "Tensor(dim: ";
  for (int i = 0; i < tsr.dim(); i++) {
    ss << tsr.size(i); 
    if (i != tsr.dim() - 1)
      ss << ", ";
  }
  ss << ")"; 
  return ss.str(); 
}


Backend::Backend() : m_loaded(0), m_device(CPU), m_use_gpu(false), m_sr(0) {
  at::init_num_threads();
}

// ---------------------------------------------------------------------------
// Helpers working on a *given* module, without touching shared state / locks.
// Used by load() to fully prepare a model before it is swapped in.
// ---------------------------------------------------------------------------

std::vector<std::string> Backend::methods_of(torch::jit::script::Module &model) {
  std::vector<std::string> methods;
  try {
    std::vector<c10::IValue> dumb_input = {};
    auto methods_from_model = model.get_method("get_methods")(dumb_input).toList();
    for (int i = 0; i < methods_from_model.size(); i++) {
      methods.push_back(methods_from_model.get(i).toStringRef());
    }
  } catch (...) {
    methods.clear();
    for (const auto &m : model.get_methods()) {
      try {
        auto method_params = model.attr(m.name() + "_params");
        methods.push_back(m.name());
      } catch (...) {
      }
    }
  }
  return methods;
}

bool Backend::module_has_method(torch::jit::script::Module &model,
                                const std::string &method_name) {
  return model.find_method(method_name).has_value();
}

std::vector<std::string> Backend::buffer_attributes_of(torch::jit::script::Module &model) {
  std::vector<std::string> buffer_names = {};
  std::string getter_name = "get_buffer_attributes";
  if (module_has_method(model, getter_name)) {
    auto model_out = model.get_method(getter_name)(empty_args(), empty_kwargs()).toList().vec();
    for (int i = 0; i < model_out.size(); i++) {
      buffer_names.push_back(model_out[i].toStringRef());
    }
  }
  return buffer_names;
}

// Must be called with m_model_mutex held.
std::vector<int> Backend::method_params_unlocked(const std::string &method) {
  std::vector<int> params;
  if (std::find(m_available_methods.begin(), m_available_methods.end(),
                method) == m_available_methods.end()) {
    return params;
  }
  try {
    auto p = m_model.attr(method + "_params").toTensor();
    auto p_int = p.to(torch::kCPU).to(torch::kInt64).contiguous();
    if (p_int.numel() < 4)
      return params;
    auto ptr = p_int.data_ptr<int64_t>();
    for (int i(0); i < 4; i++)
      params.push_back(static_cast<int>(ptr[i]));
  } catch (...) {
    params.clear();
  }
  return params;
}

// ---------------------------------------------------------------------------
// PERFORM
// Called from the compute thread (or the audio thread in no-thread mode).
// The whole call runs under m_model_mutex, so the model *and* its params are
// read atomically w.r.t. load()/reload(). Every buffer access is bounded by
// the sizes of the vectors that were passed in, so a model whose shape does
// not match the frontend buffers can produce silence, never a memory error.
// ---------------------------------------------------------------------------

static void zero_buffers(std::vector<float *> &buffers, int n_vec) {
  for (auto *buf : buffers)
    if (buf)
      memset(buf, 0, n_vec * sizeof(float));
}

void Backend::perform(std::vector<float *> &in_buffer,
                      std::vector<float *> &out_buffer, 
                      const std::string &method, 
                      int n_batches, int n_out_channels, int n_vec) {
  c10::InferenceMode guard;
  std::unique_lock<std::mutex> model_lock(m_model_mutex);

  if (!m_loaded || n_vec <= 0 || n_batches <= 0) {
    zero_buffers(out_buffer, n_vec);
    return;
  }

  auto params = method_params_unlocked(method);
  if (params.size() < 4) {
    zero_buffers(out_buffer, n_vec);
    return;
  }
  const int in_dim = params[0];
  const int in_ratio = params[1];
  const int out_dim = params[2];
  const int out_ratio = params[3];

  if (in_dim <= 0 || out_dim <= 0 || in_ratio <= 0 || out_ratio <= 0 ||
      (n_vec % in_ratio) != 0) {
    zero_buffers(out_buffer, n_vec);
    return;
  }

  try {
    // COPY BUFFERS INTO A (in_dim * n_batches, n_vec) TENSOR
    // (layout of in_buffer is channel-major: idx = c * n_batches + b)
    const int n_in = in_dim * n_batches;
    auto cat_tensor_in = torch::empty({n_in, n_vec}, torch::kFloat32);
    auto in_ptr = cat_tensor_in.data_ptr<float>();
    for (int i(0); i < n_in; i++) {
      if (i < (int)in_buffer.size() && in_buffer[i] != nullptr) {
        memcpy(in_ptr + i * n_vec, in_buffer[i], n_vec * sizeof(float));
      } else {
        memset(in_ptr + i * n_vec, 0, n_vec * sizeof(float));
      }
    }
    cat_tensor_in = cat_tensor_in.reshape({in_dim, n_batches, -1, in_ratio});
    cat_tensor_in = cat_tensor_in.select(-1, -1);
    cat_tensor_in = cat_tensor_in.permute({1, 0, 2});

    // SEND TENSOR TO DEVICE & PROCESS
    cat_tensor_in = cat_tensor_in.to(m_device);
    std::vector<torch::jit::IValue> inputs = {cat_tensor_in};
    at::Tensor tensor_out = m_model.get_method(method)(inputs).toTensor();
    tensor_out = tensor_out.repeat_interleave(out_ratio).reshape({n_batches, out_dim, -1});
    model_lock.unlock();

    if (tensor_out.size(2) != n_vec) {
      std::cerr << "model output size is not consistent, expected " << n_vec
                << " samples, got " << tensor_out.size(2) << "!\n";
      zero_buffers(out_buffer, n_vec);
      return;
    }

    tensor_out = tensor_out.to(CPU).contiguous();
    auto out_ptr = tensor_out.data_ptr<float>();

    // COPY BACK (layout of out_buffer is batch-major: idx = b * n_out_channels + c)
    for (int b(0); b < n_batches; b++) {
      for (int c(0); c < n_out_channels; c++) {
        const int idx = b * n_out_channels + c;
        if (idx >= (int)out_buffer.size() || out_buffer[idx] == nullptr)
          continue;
        if (c < out_dim) {
          memcpy(out_buffer[idx], out_ptr + (b * out_dim + c) * n_vec,
                 n_vec * sizeof(float));
        } else {
          memset(out_buffer[idx], 0, n_vec * sizeof(float));
        }
      }
    }
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
    zero_buffers(out_buffer, n_vec);
  }
}

// ---------------------------------------------------------------------------
// LOAD
// Everything is prepared on a local module (no lock held during the slow
// torch::jit::load), then model + metadata are swapped in a single critical
// section, so perform() never sees a new model with old metadata.
// ---------------------------------------------------------------------------

int Backend::load(std::string path, double sampleRate, const std::string* target_method) {
  try {
    auto model = torch::jit::load(path);
    model.eval();

    auto methods = methods_of(model);
    if (target_method != nullptr && !target_method->empty()) {
      if (std::find(methods.begin(), methods.end(), *target_method) == methods.end()) {
        throw "path " + path + " does not contain target method " + (*target_method); 
      }
    }

    c10::DeviceType device;
    {
      std::lock_guard<std::mutex> lock(m_model_mutex);
      device = m_device;
    }
    model.to(device);

    if (module_has_method(model, "set_sample_rate")) {
      try {
        auto args = empty_args(); args.push_back((int)sampleRate);
        model.get_method("set_sample_rate")(args, empty_kwargs());
      } catch (const std::exception &e) {
        std::cerr << "set_sample_rate failed for " << path << ": " << e.what() << '\n';
      }
    }

    auto buffer_attributes = buffer_attributes_of(model);

    {
      std::lock_guard<std::mutex> lock(m_model_mutex);
      m_model = model;
      m_available_methods = std::move(methods);
      m_buffer_attributes = std::move(buffer_attributes);
      m_path = path;
      m_sr = sampleRate;
      m_loaded = 1;
    }
    return 0;
  } catch (const std::exception &e) {
    throw "problem loading model " + path + ". Exception : " + e.what();
  }
}

void Backend::set_sample_rate(double sampleRate) {
  std::lock_guard<std::mutex> lock(m_model_mutex);
  if (!m_loaded || !module_has_method(m_model, "set_sample_rate"))
    return;
  m_sr = sampleRate;
  auto args = empty_args(); args.push_back((int)sampleRate);
  m_model.get_method("set_sample_rate")(args, empty_kwargs());
}

int Backend::reload() {
  std::string path; double sr;
  {
    std::lock_guard<std::mutex> lock(m_model_mutex);
    path = m_path; sr = m_sr;
  }
  return load(path, sr);
}

bool Backend::has_method(std::string method_name) {
  if (!is_loaded()){
    return false; 
  }
  std::lock_guard<std::mutex> lock(m_model_mutex);
  return module_has_method(m_model, method_name);
}

bool Backend::has_settable_attribute(std::string attribute) {
  for (const auto &a : get_settable_attributes()) {
    if (a == attribute)
      return true;
  }
  return false;
}

std::vector<std::string> Backend::get_available_methods() {
  std::lock_guard<std::mutex> lock(m_model_mutex);
  return m_available_methods;
}

std::vector<std::string> Backend::get_available_attributes() {
  std::vector<std::string> attributes;
  std::unique_lock<std::mutex> model_lock(m_model_mutex);
  for (const auto &attribute : m_model.named_attributes())
    attributes.push_back(attribute.name);
  return attributes;
}

std::vector<std::string> Backend::get_settable_attributes() {
  std::vector<std::string> attributes;
  try {
    std::vector<c10::IValue> dumb_input = {};
    std::unique_lock<std::mutex> model_lock(m_model_mutex);
    auto methods_from_model =
        m_model.get_method("get_attributes")(dumb_input).toList();
    model_lock.unlock();
    for (int i = 0; i < methods_from_model.size(); i++) {
      auto attr_name = methods_from_model.get(i).toStringRef();
      if (attr_name != "none") {
        attributes.push_back(attr_name);
      }
    }
  } catch (...) {
    std::unique_lock<std::mutex> model_lock(m_model_mutex);
    for (const auto &a : m_model.named_attributes()) {
      try {
        auto method_params = m_model.attr(a.name + "_params");
        if (a.name != "none") {
          attributes.push_back(a.name);
        }
      } catch (...) {
      }
    }
    model_lock.unlock();
  }
  return attributes;
}

std::vector<c10::IValue> Backend::get_attribute(std::string attribute_name) {
  std::string attribute_getter_name = "get_" + attribute_name;
  std::unique_lock<std::mutex> model_lock(m_model_mutex);
  try {
    auto attribute_getter = m_model.get_method(attribute_getter_name);
  } catch (...) {
    model_lock.unlock();
    throw "getter for attribute " + attribute_name + " not found in model";
  }
  std::vector<c10::IValue> getter_inputs = {}, attributes;
  auto kwargs = empty_kwargs();
  try {
    try {
      auto output_list = m_model.get_method(attribute_getter_name)(getter_inputs, kwargs).toList();
      attributes = output_list.vec();
    } catch (...) {
      auto output_tuple_ref =
          m_model.get_method(attribute_getter_name)(getter_inputs, kwargs).toTuple();
      attributes = (*output_tuple_ref.get()).elements();
    }
  } catch (...) {
    model_lock.unlock();
    throw ("could not access method " + attribute_getter_name + " from original script.");
  }
  model_lock.unlock();
  return attributes;
}

std::string Backend::get_attribute_as_string(std::string attribute_name) {
  std::vector<c10::IValue> getter_outputs = get_attribute(attribute_name);
  // finstringd arguments
  torch::Tensor setter_params;
  std::unique_lock<std::mutex> model_lock(m_model_mutex);
  try {
    setter_params = m_model.attr(attribute_name + "_params").toTensor();
    model_lock.unlock();
  } catch (...) {
    model_lock.unlock();
    throw "parameters to get attribute " + attribute_name +
        " not found in model";
  }
  std::string current_attr = "";
  for (int i = 0; i < setter_params.size(0); i++) {
    int current_id = setter_params[i].item().toInt();
    switch (current_id) {
    // bool case
    case 0: {
      current_attr += (getter_outputs[i].toBool()) ? "true" : "false";
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
    // tensor case
    case 4: {
      auto tensor = getter_outputs[i].toTensor();
      current_attr += tensor_to_str(tensor);
      break; 
    }
    case 5: {
      current_attr += getter_outputs[i].toStringRef();
      break;
    }
    default: {
      throw "bad type id : " + std::to_string(current_id) + " at index " +
          std::to_string(i);
      break;
    }
    }
    if (i < setter_params.size(0) - 1)
      current_attr += " ";
  }
  return current_attr;
}


void Backend::set_attribute(std::string attribute_name,
                            std::vector<std::string> attribute_args, 
                            const Backend::BufferMap &buffer_array) {
  // find setter
  std::string attribute_setter_name = "set_" + attribute_name;
  std::unique_lock<std::mutex> model_lock(m_model_mutex);
  try {
    auto attribute_setter = m_model.get_method(attribute_setter_name);
  } catch (...) {
    model_lock.unlock();
    throw "setter for attribute " + attribute_name + " not found in model";
  }
  // find arguments
  torch::Tensor setter_params;
  try {
    setter_params = m_model.attr(attribute_name + "_params").toTensor();
  } catch (...) {
    model_lock.unlock();
    throw "parameters to set attribute " + attribute_name +
        " not found in model";
  }
  model_lock.unlock();

  // check attribute params
  int setter_elements = setter_params.numel();
  if (attribute_args.size() != setter_elements) {
    std::stringstream exception;
    exception << "wrong number of elements : setter seems to have " << std::to_string(setter_params.numel());
    exception << ", but " << attribute_args.size() << " was given";
    throw exception.str();
  }

  // process inputs
  std::vector<c10::IValue> setter_inputs = {};
  auto n_args = setter_params.size(0);
  for (int i = 0; i < n_args; i++) {
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
      // tensor case
      case 4: {
        auto buffer_name = get_buffer_name(attribute_name, i);
        if (!buffer_array.contains(buffer_name)) {
          throw std::string("buffer for argument ") + buffer_name + std::string(" not found. Did you initialise it?");
        } else {
          auto buffer = buffer_array.at(buffer_name);
          auto buf_tensor = buffer.to_tensor().index({0});
          setter_inputs.push_back(buf_tensor.clone()); 
        }
        break;
      }
      case 5: {
        auto buffer_name = get_buffer_name(attribute_name, i);
        if (!buffer_array.contains(buffer_name)) {
          throw std::string("buffer for argument ") + buffer_name + std::string(" not found. Did you initialise it?");
        } else {
          auto buffer = buffer_array.at(buffer_name);
          update_buffer(buffer_name, buffer); 
          setter_inputs.push_back(c10::IValue(buffer_name));
        }
        break;
      }
      default:
        throw "bad type id : " + std::to_string(current_id) + "at index " +
            std::to_string(i);
        break;
    }
  }
  try {
    std::unique_lock<std::mutex> model_lock(m_model_mutex);
    auto kwargs = empty_kwargs();
    auto setter_out = m_model.get_method(attribute_setter_name)(setter_inputs, kwargs);
    model_lock.unlock();
    int setter_result = setter_out.toInt();
    if (setter_result != 0) {
      throw "setter returned -1";
    }
  } catch (std::string &e) {
    throw "setter for " + attribute_name + " failed : " + e;
  } catch (std::exception &e) {
    throw "setter for " + attribute_name + " failed : " + e.what();
  } catch (...) {
    throw "setter for " + attribute_name + " failed";
  }
}

std::vector<int> Backend::get_method_params(std::string method) {
  std::lock_guard<std::mutex> lock(m_model_mutex);
  return method_params_unlocked(method);
}

int Backend::get_higher_ratio() {
  std::lock_guard<std::mutex> lock(m_model_mutex);
  int higher_ratio = 1;
  for (const auto &method : m_available_methods) {
    auto params = method_params_unlocked(method);
    if (params.size() < 4)
      continue; // METHOD NOT USABLE, SKIPPING
    int max_ratio = std::max(params[1], params[3]);
    higher_ratio = std::max(higher_ratio, max_ratio);
  }
  return higher_ratio;
}

bool Backend::is_loaded() { return m_loaded; }

void Backend::use_gpu(bool value) {
  std::unique_lock<std::mutex> model_lock(m_model_mutex);
  if (value) {
    if (torch::hasCUDA()) {
      std::cout << "sending model to cuda" << std::endl;
      m_device = CUDA;
    } else if (torch::hasMPS()) {
      std::cout << "sending model to mps" << std::endl;
      m_device = MPS;
    } else {
      std::cout << "sending model to cpu" << std::endl;
      m_device = CPU;
    }
  } else {
    m_device = CPU;
  }
  m_model.to(m_device);
}

std::string Backend::get_buffer_name(std::string attribute_name, int attribute_elt_idx) {
  return attribute_name + "#" + std::to_string(attribute_elt_idx);
}

bool Backend::is_buffer_element_of_attribute(std::string attribute_name, int attribute_elt_idx) {
  std::string target_buffer_name = get_buffer_name(attribute_name, attribute_elt_idx);
  std::lock_guard<std::mutex> lock(m_model_mutex);
  for (auto buffer_name: m_buffer_attributes) {
    // std::cout << "target buffer name : " << target_buffer_name << "; compared buffer name : " << buffer_name << std::endl;
    if (buffer_name == target_buffer_name) {
      return true;
    }
  }
  return false;
}

bool Backend::is_tensor_element_of_attribute(std::string attribute_name, int attribute_elt_idx) {
  std::lock_guard<std::mutex> lock(m_model_mutex);
  auto attribute_params = m_model.attr(attribute_name + "_params").toTensor(); 
  if (id_to_string_hash.at(attribute_params.item().toInt()) == "tensor") {
    return true;
  } else {
    return false;
  }
}

std::vector<std::string> Backend::get_buffer_attributes() {
  std::lock_guard<std::mutex> lock(m_model_mutex);
  return m_buffer_attributes;
}

int Backend::reset_buffer(std::string buffer_name) {
  std::string clear_method = "clear_buffer_attribute";
  auto args = empty_args(); auto kwargs = empty_kwargs();
  args.push_back(buffer_name);
  std::unique_lock<std::mutex> model_lock(m_model_mutex);
  auto model_out = m_model.get_method(clear_method)(args, kwargs);
  int result = model_out.toInt();
  return result;  
}

int Backend::update_buffer(std::string buffer_name, StaticBuffer<float> &buffer) {
  std::string setter_method = "set_buffer_attribute";
  auto args = empty_args(); auto kwargs = empty_kwargs();
  auto sr = static_cast<int>(buffer.sr());
  std::unique_lock<std::mutex> model_lock(m_model_mutex);
  auto tensor = buffer.to_tensor().to(m_device);
  args.push_back(buffer_name); args.push_back(tensor); args.push_back(sr);
  auto model_out = m_model.get_method(setter_method)(args, kwargs);
  model_lock.unlock();
  int result = model_out.toInt();
  return result; 
}

ModelInfo Backend::get_model_info() {
  if (!m_loaded) {
    throw "no model loaded; cannot retrieve model information";
  }

  auto methods = get_available_methods();
  auto attributes = get_settable_attributes();
  auto method_dict = ModelInfo::MethodDict();
  auto attribute_dict = ModelInfo::AttributeDict();
  std::unique_lock<std::mutex> model_lock(m_model_mutex);
  model_lock.unlock();
  try {
    for (auto method : methods) {
      auto method_properties = MethodProperties();
      model_lock.lock();
      auto setter_params = m_model.attr(method + "_params").toTensor();
      model_lock.unlock();
      method_properties.name = method; 
      method_properties.channels_in = setter_params[0].item().toInt(); 
      method_properties.ratio_in = setter_params[1].item().toInt(); 
      method_properties.channels_out = setter_params[2].item().toInt(); 
      method_properties.ratio_out = setter_params[3].item().toInt(); 
      method_dict.insert({method, method_properties});
    }
    for (auto attribute: attributes) {
      model_lock.lock();
      auto attribute_params = m_model.attr(attribute + "_params").toTensor();
      model_lock.unlock();
      auto attribute_properties = AttributeProperties();
      attribute_properties.name = attribute;
      auto attribute_types = std::vector<std::string>();
      for (int i = 0; i < attribute_params.size(0); i++) {
        auto param_idx = attribute_params[i].item().toInt();
        auto param_str = this->id_to_string_hash.at(param_idx);
        attribute_types.push_back(param_str);
      }
      attribute_properties.attribute_types = attribute_types;
      attribute_dict.insert({attribute, attribute_properties});
    }
    auto model_info = ModelInfo();
    model_info.method_properties = method_dict;
    model_info.attribute_properties = attribute_dict;
    return model_info;
  } catch (std::exception& e) {
    model_lock.unlock();
    throw "error fetching model information for model " + m_path + ". Caught error : " + e.what();
  }
}