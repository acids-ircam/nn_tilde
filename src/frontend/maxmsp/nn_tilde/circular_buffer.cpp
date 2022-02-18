#include "circular_buffer.h"
#include <algorithm>

template <class T>
circular_buffer<T>::circular_buffer(size_t size)
    : _buffer(std::make_unique<T[]>(size), _max_size(size)) {}

template <class T> bool circular_buffer<T>::empty() {
  return (!_full && _head == _tail);
}

template <class T> bool circular_buffer<T>::full() { return _full; }

template <class T> void circular_buffer<T>::write(T *input_array, int N) {
  std::lock_guard<std::mutex> guard(_mutex);
  while (N--) {
    _buffer[_head] = *(input_array++);
    if (_full)
      _tail = (_tail + 1) % _max_size;
    _head = (_head + 1) % _max_size;
    _full = _head == _tail;
  }
}

template <class T> void circular_buffer<T>::read(T *output_array, int N) {
  std::lock_guard<std::mutex> guard(_mutex);
  while (N--) {
    if (empty()) {
      *(output_array++) = T();
    } else {
      *(output_array++) = _buffer[_tail];
      _tail = (_tail + 1) % _max_size;
      _full = false;
    }
  }
}

template <class T> void circular_buffer<T>::reset() {
  std::lock_guard<std::mutex> guard(_mutex);
  _head = _tail;
  _count = 0;
}