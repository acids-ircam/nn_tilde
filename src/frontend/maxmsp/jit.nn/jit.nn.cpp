#include "c74_min.h"
#include "shared_tensor.hpp"
#include <algorithm>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <chrono>
#include <string>
#include <vector>

#ifndef VERSION
#define VERSION "UNDEFINED"
#endif

using namespace c74::min;
namespace ipc = boost::interprocess;

long position_to_index(long x_pos, long y_pos, long height, long plane_count) {
  return y_pos * plane_count + x_pos * height * plane_count;
}

int check_index_overflow(long index) {
  if (index >= X_DIM * Y_DIM * PLANE_COUNT)
    return 1;
  else
    return 0;
}

class jit_nn : public object<jit_nn>, public matrix_operator<> {
public:
  MIN_DESCRIPTION{"Interface for deep learning models"};
  MIN_TAGS{"deep learning, ai"};
  MIN_AUTHOR{"Antoine Caillon & Axel Chemla--Romeu-Santos"};

  inlet<> input{this, "(matrix) Input", "matrix"};
  outlet<> output{this, "(matrix) Output", "matrix"};

  jit_nn(const atoms &args = {});
  void setup_stream(std::string stream_name);

  attribute<symbol> attribute_stream_name{
      this, "stream", "",
      setter{[this](const c74::min::atoms &args,
                    const int inlet) -> c74::min::atoms {
        std::cout << args[0] << std::endl;
        setup_stream(std::string(args[0]));
        return args;
      }},
      description{"Define jit.nn stream name"}};

  template <class matrix_type, size_t plane_count>
  cell<matrix_type, plane_count> calc_cell(cell<matrix_type, plane_count> input,
                                           const matrix_info &info,
                                           matrix_coord &position);

protected:
  ShmRemove input_stream_handler, output_stream_handler;
  ipc::shared_memory_object shm_input, shm_output;
  ipc::mapped_region region_input, region_output;
  Matrix *matrix_input, *matrix_output;

  template <class matrix_type, size_t plane_count>
  cell<matrix_type, plane_count> get_cell(const matrix_info &info, int x,
                                          int y);
};

jit_nn::jit_nn(const atoms &args) : matrix_operator(false) {}

void jit_nn::setup_stream(std::string stream_name) {
  if (stream_name == "")
    return;

  std::string stream_input = stream_name + "_input";
  std::string stream_output = stream_name + "_output";

  input_stream_handler = ShmRemove(stream_input);
  output_stream_handler = ShmRemove(stream_output);

  shm_input = ipc::shared_memory_object(ipc::create_only, stream_input.c_str(),
                                        ipc::read_write);
  shm_output = ipc::shared_memory_object(
      ipc::create_only, stream_output.c_str(), ipc::read_write);

  shm_input.truncate(sizeof(Matrix));
  shm_output.truncate(sizeof(Matrix));

  region_input = ipc::mapped_region(shm_input, ipc::read_write);
  region_output = ipc::mapped_region(shm_output, ipc::read_write);

  matrix_input = new (region_input.get_address()) Matrix;
  matrix_output = new (region_output.get_address()) Matrix;
}

template <class matrix_type, size_t plane_count>
cell<matrix_type, plane_count>
jit_nn::calc_cell(cell<matrix_type, plane_count> input, const matrix_info &info,
                  matrix_coord &position) {
  cell<matrix_type, plane_count> output;

  if (std::string(attribute_stream_name.get()) == "none")
    return output;

  long width = info.width();
  long height = info.height();

  width = width <= 0 ? 1 : width <= X_DIM ? width : X_DIM;
  height = height <= 0 ? 1 : height <= Y_DIM ? height : Y_DIM;

  if (position.x() == 0 && position.y() == 0) {
    matrix_input->width = width;
    matrix_output->width = width;
    matrix_input->height = height;
    matrix_output->height = height;

    for (long x(0); x < width; x++) {
      for (long y(0); y < height; y++) {
        auto cell = info.in_cell<matrix_type, plane_count>(x, y);
        auto pixel_index = position_to_index(x, y, height, PLANE_COUNT);

        for (int plane(0); plane < info.plane_count(); plane++) {
          matrix_input->values[pixel_index + plane] = float(cell[plane]);
        }
      }
    }
    matrix_input->lock.release(); // SIGNAL MODEL THAT DATA IS READY
    matrix_output->lock.try_acquire_for(std::chrono::milliseconds(50));
  }

  long pixel_index =
      position_to_index(position.x(), position.y(), height, info.plane_count());

  // TRANSFER DATA FROM MODEL OUTPUT
  for (int plane(0); plane < info.plane_count(); plane++) {
    if (check_index_overflow(pixel_index + plane))
      break;
    output[plane] = matrix_type(matrix_output->values[pixel_index + plane]);
  }
  return output;
}

template <class matrix_type, size_t plane_count>
cell<matrix_type, plane_count> jit_nn::get_cell(const matrix_info &info, int x,
                                                int y) {
  auto x1 = clamp<decltype(x)>(x, 0, info.width() - 1);
  auto y1 = clamp<decltype(y)>(y, 0, info.height() - 1);
  auto c = info.in_cell<matrix_type, plane_count>(x1, y1);
  auto tesdt = info.m_bip;
  return c;
}

MIN_EXTERNAL(jit_nn);