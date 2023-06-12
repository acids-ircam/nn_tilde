#include "c74_min.h"
#include "shared_tensor.hpp"
#include <algorithm>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <mutex>
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

  template <class matrix_type, size_t plane_count>
  cell<matrix_type, plane_count> calc_cell(cell<matrix_type, plane_count> input,
                                           const matrix_info &info,
                                           matrix_coord &position) {
    long width = info.width();
    long height = info.height();

    width = width <= 0 ? 1 : width <= X_DIM ? width : X_DIM;
    height = height <= 0 ? 1 : height <= Y_DIM ? height : Y_DIM;

    if (position.x() == 0 && position.y() == 0) {
      matrix_input->width = width;
      matrix_output->width = width;
      matrix_input->height = height;
      matrix_output->height = height;
    }

    cell<matrix_type, plane_count> output;
    long pixel_index = position_to_index(position.x(), position.y(), height,
                                         info.plane_count());

    for (int plane(0); plane < info.plane_count(); plane++) {
      if (check_index_overflow(pixel_index + plane))
        break;
      matrix_input->values[pixel_index + plane] = float(input[plane]);
      output[plane] = matrix_type(matrix_output->values[pixel_index + plane]);
    }

    return output;
  }

protected:
  ShmRemove input_stream_handler, output_stream_handler;
  ipc::shared_memory_object shm_input, shm_output;
  ipc::mapped_region region_input, region_output;
  Matrix *matrix_input, *matrix_output;
};

jit_nn::jit_nn(const atoms &args) : matrix_operator(false) {
  std::string stream_input = std::string("debug") + "_input";
  std::string stream_output = std::string("debug") + "_output";
  input_stream_handler = ShmRemove(stream_input);
  output_stream_handler = ShmRemove(stream_output);

  shm_input = ipc::shared_memory_object(ipc::create_only, "debug_input",
                                        ipc::read_write);
  shm_output = ipc::shared_memory_object(ipc::create_only, "debug_output",
                                         ipc::read_write);

  shm_input.truncate(sizeof(Matrix));
  shm_output.truncate(sizeof(Matrix));

  region_input = ipc::mapped_region(shm_input, ipc::read_write);
  region_output = ipc::mapped_region(shm_output, ipc::read_write);

  matrix_input = new (region_input.get_address()) Matrix;
  matrix_output = new (region_output.get_address()) Matrix;
}

MIN_EXTERNAL(jit_nn);