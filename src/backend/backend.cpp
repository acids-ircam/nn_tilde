#include "backend.h"
#include <exception>
#include "parsing_utils.h"
#include <algorithm>
#include <iostream>
#include <exception>
#include <stdlib.h>

#define CPU torch::kCPU
#define CUDA torch::kCUDA
#define MPS torch::kMPS

Backend::Backend() : m_loaded(0), m_device(CPU), m_use_gpu(false) {
  at::init_num_threads();
}

void Backend::perform(std::vector<float *> in_buffer,
                      std::vector<float *> out_buffer, 
                      int n_vec, std::string method, int n_batches) {
  c10::InferenceMode guard;

  auto params = get_method_params(method);
  // std::cout << "in_buffer length : " << in_buffer.size() << std::endl;
  // std::cout << "out_buffer length : " << out_buffer.size() << std::endl;

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
  // for (auto buf : in_buffer)
  for (int i(0); i < in_dim * n_batches; i++) {
    if (i < in_buffer.size()) {
      tensor_in.push_back(torch::from_blob(in_buffer[i], {1, 1, n_vec}));
    } else {
      tensor_in.push_back(torch::zeros({1, 1, n_vec}));
    }
    // std::cout << i << " : " << tensor_in[i].min().item<float>() << std::endl;
  }

  auto cat_tensor_in = torch::cat(tensor_in, 1);
  cat_tensor_in = cat_tensor_in.reshape({in_dim, n_batches, -1, in_ratio});
  cat_tensor_in = cat_tensor_in.select(-1, -1);
  cat_tensor_in = cat_tensor_in.permute({1, 0, 2});
  // std::cout << cat_tensor_in.size(0) << ";" << cat_tensor_in.size(1) << ";" << cat_tensor_in.size(2) << std::endl;
  // for (int i = 0; i < cat_tensor_in.size(1); i++ ) 
    // std::cout << cat_tensor_in[0][i][0] << ";";
  // std::cout << std::endl;

  // SEND TENSOR TO DEVICE
  std::unique_lock<std::mutex> model_lock(m_model_mutex);
  cat_tensor_in = cat_tensor_in.to(m_device);
  std::vector<torch::jit::IValue> inputs = {cat_tensor_in};
  auto kwargs = empty_kwargs();

  // PROCESS TENSOR
  at::Tensor tensor_out;
  try {
    tensor_out = m_model.get_method(method)(inputs, kwargs).toTensor();
    tensor_out = tensor_out.repeat_interleave(out_ratio).reshape(
        {n_batches, out_dim, -1});
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
    return;
  }
  model_lock.unlock();

  int out_batches(tensor_out.size(0)), out_channels(tensor_out.size(1)),
      out_n_vec(tensor_out.size(2));

  // for (int b(0); b < out_batches; b++) {
  //   for (int c(0); c < out_channels; c++) {
  //     std::cout << b << ";" << c << ";" << tensor_out[b][c].min().item<float>() << std::endl;
  //   }
  // }

  // CHECKS ON TENSOR SHAPE
  // if (out_batches * out_channels != out_buffer.size()) {
  //   std::cout << "bad out_buffer size, expected " << out_batches * out_channels
  //             << " buffers, got " << out_buffer.size() << "!\n";
  //   return;
  // }

  if (out_n_vec != n_vec) {
    std::cout << "model output size is not consistent, expected " << n_vec
              << " samples, got " << out_n_vec << "!\n";
    return;
  }

  tensor_out = tensor_out.to(CPU);
  // tensor_out = tensor_out.reshape({out_batches * out_channels, -1});

  int out_buffer_dim = out_buffer.size() / n_batches;
  for (int i(0); i < out_buffer_dim; i++) {
    for (int j(0); j < n_batches; j++) {
      if (i < tensor_out.size(1)) {
        auto out_ptr = tensor_out.index({j, i}).contiguous().data_ptr<float>();
        memcpy(out_buffer[j * out_buffer_dim + i], out_ptr, n_vec * sizeof(float));
      } else {
        // put zeros
        memset(out_buffer[j * out_buffer_dim + i], 0, n_vec *sizeof(float));
      }
    }
  }
}

int Backend::load(std::string path, double sampleRate, const std::string* target_method) {
  try {
    auto model = torch::jit::load(path);
    if (target_method != nullptr) {
      // if target_method is not null, check if loaded model has it
      auto locked_model = LockedModel(); 
      locked_model.model = &model;  
      auto methods = get_available_methods(&locked_model);
      if (std::find(methods.begin(), methods.end(), *target_method) == methods.end()) {
        throw "path " + path + "does not contain target method " + (*target_method); 
      }
    }
    model.eval();
    model.to(m_device);

    std::unique_lock<std::mutex> model_lock(m_model_mutex);
    m_model = model;
    model_lock.unlock();

    m_available_methods = get_available_methods();
    m_buffer_attributes = retrieve_buffer_attributes();
    m_path = path;
    set_sample_rate(sampleRate);
    m_loaded = 1;
    return 0;
  } catch (const std::exception &e) {
    throw "problem loading model " + path + ". Exception : " + e.what();
  }
}

void Backend::set_sample_rate(double sampleRate) {
 if (has_method("set_sample_rate")) {
      m_sr = sampleRate;
      std::vector<c10::IValue> sr_input = {sampleRate};
      std::unique_lock<std::mutex> model_lock(m_model_mutex);
      auto args = empty_args(); args.push_back((int)sampleRate);
      auto kwargs = empty_kwargs();
      std::string method_name = "set_sample_rate";
      m_model.get_method(method_name)(args, kwargs); 
      model_lock.unlock();
    } 
}

int Backend::reload() {
  auto return_code = load(m_path, m_sr);
  return return_code;
}

bool Backend::has_method(std::string method_name) {
  std::unique_lock<std::mutex> model_lock(m_model_mutex);
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

std::vector<std::string> Backend::get_available_methods(LockedModel *target_model) {
  std::vector<std::string> methods;
  torch::jit::script::Module *model;
  std::mutex *mutex; 
  if (target_model == nullptr) {
    model = &m_model;
    mutex = &m_model_mutex;    
  } else {
    model = target_model->model;
    mutex = &target_model->mutex;
  }
  try {
    std::vector<c10::IValue> dumb_input = {};
    std::unique_lock<std::mutex> model_lock(*mutex);
    auto methods_from_model =
        model->get_method("get_methods")(dumb_input).toList();
    model_lock.unlock();

    for (int i = 0; i < methods_from_model.size(); i++) {
      methods.push_back(methods_from_model.get(i).toStringRef());
    }
  } catch (...) {
    std::unique_lock<std::mutex> model_lock(*mutex);
    for (const auto &m : model->get_methods()) {
      try {
        auto method_params = model->attr(m.name() + "_params");
        methods.push_back(m.name());
      } catch (...) {
      }
    }
    model_lock.unlock();
  }
  return methods;
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
      // buffer case
      case 5: {
        auto buffer_name = get_buffer_name(attribute_name, i);
        if (!buffer_array.contains(buffer_name)) {
          throw std::string("buffer for argument ") + buffer_name + std::string(" not found. Did you initialise it?");
        } else {
          auto buffer = buffer_array.at(buffer_name);
          // make buffer object
          auto tensor = 2 * torch::ones_like(buffer.to_tensor()).to(m_device);
          auto sr = static_cast<int>(buffer.sr());
          auto buffer_input = c10::ivalue::Tuple::create(tensor, sr);
          // append buffer to inputs
          auto min = tensor.min(); 
          auto max = tensor.max(); 
          setter_inputs.push_back(buffer_input);
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
  } catch (...) {
    throw "setter for " + attribute_name + " failed";
  }
}

std::vector<int> Backend::get_method_params(std::string method) {
  std::vector<int> params;

  if (std::find(m_available_methods.begin(), m_available_methods.end(),
                method) != m_available_methods.end()) {
    try {
      std::unique_lock<std::mutex> model_lock(m_model_mutex);
      auto p = m_model.attr(method + "_params").toTensor();
      model_lock.unlock();
      for (int i(0); i < 4; i++)
        params.push_back(p[i].item().to<int>());
    } catch (...) {
    }
  }
  return params;
}

int Backend::get_higher_ratio() {
  int higher_ratio = 1;
  for (const auto &method : m_available_methods) {
    auto params = get_method_params(method);
    if (!params.size())
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
  for (auto buffer_name: m_buffer_attributes) {
    if (buffer_name == target_buffer_name) {
      return true;
    }
  }
  return false;
}

std::vector<std::string> Backend::retrieve_buffer_attributes() {
  std::vector<std::string> bufferNames = {};
  std::string getter_name = "get_buffer_attributes";
  if (has_method(getter_name)) {
    std::unique_lock<std::mutex> model_lock(m_model_mutex);
    try {
      auto model_method = m_model.get_method(getter_name);
      auto model_out = model_method(empty_args(), empty_kwargs()).toList().vec();
      for (int i = 0; i < model_out.size(); i++) {
        bufferNames.push_back(model_out[i].toStringRef());
      }
      model_lock.unlock();
    } catch (std::exception& e) {
      model_lock.unlock();
      throw e;
    }
  } 
  return bufferNames;
}

std::vector<std::string> Backend::get_buffer_attributes() {
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
  auto tensor = buffer.to_tensor(); auto sr = static_cast<int>(buffer.sr());
  auto tensor_min = tensor.min(); auto tensor_max = tensor.max(); 
  auto tensor_size = {tensor.size(0), tensor.size(1)};
  args.push_back(buffer_name); args.push_back(tensor); args.push_back(sr);
  std::unique_lock<std::mutex> model_lock(m_model_mutex);
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