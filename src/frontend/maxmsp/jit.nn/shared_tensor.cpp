#include "shared_tensor.hpp"

#include <algorithm>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <iostream>
#include <string>
#include <torch/extension.h>
#include <torch/torch.h>

namespace ipc = boost::interprocess;
namespace py = pybind11;

struct SharedTensor {
  SharedTensor(std::string key) {
    m_shm_input = ipc::shared_memory_object(
        ipc::open_only, (key + "_input").c_str(), ipc::read_write);
    m_shm_output = ipc::shared_memory_object(
        ipc::open_only, (key + "_output").c_str(), ipc::read_write);

    m_region_input = ipc::mapped_region(m_shm_input, ipc::read_write);
    m_region_output = ipc::mapped_region(m_shm_output, ipc::read_write);

    matrix_input = static_cast<Matrix *>(m_region_input.get_address());
    matrix_output = static_cast<Matrix *>(m_region_output.get_address());
  };

  torch::Tensor get_shared_tensor() {
    return torch::from_blob(matrix_input->values, {X_DIM, Y_DIM, PLANE_COUNT},
                            torch::TensorOptions().dtype(torch::kFloat32))
        .clone()
        .permute({1, 0, 2});
  };

  void set_shared_tensor(torch::Tensor tensor) {
    memcpy(matrix_output->values,
           tensor.permute({1, 0, 2}).contiguous().data_ptr<float>(),
           X_DIM * Y_DIM * PLANE_COUNT * sizeof(float));
  };

  ipc::shared_memory_object m_shm_input, m_shm_output;
  ipc::mapped_region m_region_input, m_region_output;
  Matrix *matrix_input, *matrix_output;
};

PYBIND11_MODULE(TORCH_EXTENSION_NAME, m) {
  py::class_<SharedTensor>(m, "SharedTensor")
      .def(py::init<const std::string &>())
      .def("get_shared_tensor", &SharedTensor::get_shared_tensor)
      .def("set_shared_tensor", &SharedTensor::set_shared_tensor);
}