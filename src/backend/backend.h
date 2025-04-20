#pragma once
#include <mutex>
#include <string>
#include <torch/script.h>
#include "../shared/static_buffer.h"
#include <torch/torch.h>
#include <vector>



struct MethodProperties {
  std::string name = ""; 
  int channels_in = -1;
  int channels_out = -1;
  int ratio_in = -1;
  int ratio_out = -1;
};

struct AttributeProperties {
  std::string name = ""; 
  std::vector<std::string> attribute_types = {};
};

struct ModelInfo {
  using MethodDict = std::unordered_map<std::string, MethodProperties>;
  using AttributeDict = std::unordered_map<std::string, AttributeProperties>;
  MethodDict method_properties = {};
  AttributeDict attribute_properties = {}; 
};

struct LockedModel {
  torch::jit::script::Module* model; 
  std::mutex mutex;
};


class Backend {

protected:
  torch::jit::script::Module m_model;
  int m_loaded, m_in_dim, m_in_ratio, m_out_dim, m_out_ratio;
  std::string m_path;
  std::mutex m_model_mutex;
  std::vector<std::string> m_available_methods;
  std::vector<std::string> m_buffer_attributes;
  c10::DeviceType m_device;
  bool m_use_gpu;
  std::vector<std::string> retrieve_buffer_attributes();
  std::unique_ptr<std::thread> set_attribute_thread; 
  double m_sr; 

public:
  using DataType = float; 
  using ArgsType = std::vector<c10::IValue>;
  using KwargsType = std::unordered_map<std::string, c10::IValue>;
  using BufferMap = std::map<std::string, StaticBuffer<float>>;

  Backend();
  void perform(std::vector<float *> &in_buffer,
                      std::vector<float *> &out_buffer, 
                      std::string method, 
                      int n_batches, int n_out_channels, int n_vec);
  bool has_method(std::string method_name);
  bool has_settable_attribute(std::string attribute);
  std::vector<std::string> get_available_methods(LockedModel *model = nullptr);
  std::vector<std::string> get_available_attributes();
  std::vector<std::string> get_settable_attributes();
  std::vector<c10::IValue> get_attribute(std::string attribute_name);
  std::string get_attribute_as_string(std::string attribute_name);
  void set_attribute(std::string attribute_name,
                     std::vector<std::string> attribute_args, 
                     const Backend::BufferMap &buffer_array);

  // buffer attributes
  bool is_buffer_element_of_attribute(std::string attribute_name, int attribute_elt_idx);
  bool is_tensor_element_of_attribute(std::string attribute_name, int attribute_elt_idx);
  // auto get_buffer_attribtues() { return m_buffer_attributes; }
  std::string get_buffer_name(std::string attribute_name, int attribute_elt_idx);
  int update_buffer(std::string buffer_id, StaticBuffer<float> &buffer);
  int reset_buffer(std::string);

  std::vector<int> get_method_params(std::string method);
  int get_higher_ratio();
  int load(std::string path, double sampleRate, const std::string* target_method = nullptr);
  int reload();
  void set_sample_rate(double sampleRate);
  bool is_loaded();
  torch::jit::script::Module get_model() { return m_model; }
  void use_gpu(bool value);
  std::vector<std::string> get_buffer_attributes();
  
  ArgsType empty_args() { return ArgsType(); }
  KwargsType empty_kwargs() { return KwargsType(); }
  std::pair<ArgsType, KwargsType> empty_inputs() {
    return std::make_pair(empty_args(), empty_kwargs());
  } 

  ModelInfo get_model_info();
  const std::unordered_map<int, std::string> id_to_string_hash = {
    {0, "bool"}, 
    {1, "int"}, 
    {2, "float"},
    {3, "string"}, 
    {4, "tensor"}, 
    {5, "buffer"} 
  };
};
