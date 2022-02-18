#pragma once
#include <memory>
#include <mutex>

template <class T> class circular_buffer {
public:
  circular_buffer(size_t size);
  bool empty();
  bool full();
  void write(T *input_array, int N);
  void read(T *output_array, int N);
  void reset();

protected:
  std::unique_ptr<T[]> _buffer;
  size_t _max_size;
  std::mutex _mutex;

  int _head = 0;
  int _tail = 0;
  int _count = 0;
  bool _full = false;
};