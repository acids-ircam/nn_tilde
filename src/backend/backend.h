#pragma once
#include <mutex>
#include <string>
#include <torch/script.h>
#include <torch/torch.h>
#include <vector>

class Backend {
protected:
  torch::jit::script::Module m_model;
  int m_loaded;
  std::string m_path;
  std::mutex m_model_mutex;
  std::vector<std::string> m_available_methods;
  c10::DeviceType m_device;
  bool m_use_gpu;

public:
  Backend();
  void perform(std::vector<float *> in_buffer, std::vector<float *> out_buffer,
               int n_vec, std::string method, int n_batches);
  bool has_method(std::string method_name);
  bool has_settable_attribute(std::string attribute);
  std::vector<std::string> get_available_methods();
  std::vector<std::string> get_available_attributes();
  std::vector<std::string> get_settable_attributes();
  std::vector<c10::IValue> get_attribute(std::string attribute_name);
  std::string get_attribute_as_string(std::string attribute_name);
  void set_attribute(std::string attribute_name,
                     std::vector<std::string> attribute_args);

  std::vector<int> get_method_params(std::string method);
  int get_higher_ratio();
  int load(std::string path);
  int reload();
  bool is_loaded();
  torch::jit::script::Module get_model() { return m_model; }
  void use_gpu(bool value);
};
