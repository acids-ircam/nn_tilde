#pragma once
#include <string>
#include <torch/script.h>
#include <torch/torch.h>
#include <vector>
#include <mutex>

class Backend {
private:
  torch::jit::script::Module m_model;
  int m_loaded;
  std::string m_filepath;

  // This is a test
  static std::mutex m_render;

public:
  Backend();
  void perform(std::vector<float *> in_buffer, std::vector<float *> out_buffer,
               int n_vec, std::string method, int n_batches);
  std::vector<std::string> get_available_methods();
  std::vector<std::string> get_available_attributes();
  std::vector<int> get_method_params(std::string method);
  int get_higher_ratio();
  int load(std::string path);
  bool is_loaded();
  bool m_cuda_available;
  torch::jit::script::Module get_model() { return m_model; }
};
