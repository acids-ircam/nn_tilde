#pragma once
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>

template <class InType, class OutType> class circular_buffer {
public:
  circular_buffer();
  void initialize(size_t size);
  bool empty();
  bool full();
  void put(InType *input_array, int N);
  void get(OutType *output_array, int N);
  void reset();

protected:
  std::unique_ptr<OutType[]> _buffer;
  size_t _max_size;
  std::mutex _mutex;
  std::condition_variable _cv;

  int _head = 0;
  int _tail = 0;
  int _count = 0;
  bool _full = false;
};

template <class InType, class OutType>
circular_buffer<InType, OutType>::circular_buffer() {}

template <class InType, class OutType>
void circular_buffer<InType, OutType>::initialize(size_t size) {
  _buffer = std::make_unique<OutType[]>(size);
  _max_size = size;
}

template <class InType, class OutType>
bool circular_buffer<InType, OutType>::empty() {
  return (!_full && _head == _tail);
}

template <class InType, class OutType>
bool circular_buffer<InType, OutType>::full() {
  return _full;
}

template <class InType, class OutType>
void circular_buffer<InType, OutType>::put(InType *input_array, int N) {
  if (!_max_size)
    return;

  while (N--) {
    std::unique_lock<std::mutex> lock(_mutex);
    _cv.wait_for(lock, std::chrono::milliseconds(10),
                 [this] { return !this->_full; });

    _buffer[_head] = OutType(*(input_array++));
    _head = (_head + 1) % _max_size;
    _full = _head == _tail;
    lock.unlock();
  }
}

template <class InType, class OutType>
void circular_buffer<InType, OutType>::get(OutType *output_array, int N) {
  if (!_max_size)
    return;

  while (N--) {
    std::lock_guard<std::mutex> lock(_mutex);

    if (empty()) {
      *(output_array++) = OutType();
    } else {
      *(output_array++) = _buffer[_tail];
      _tail = (_tail + 1) % _max_size;
      _full = false;
      _cv.notify_one();
    }
  }
}

template <class InType, class OutType>
void circular_buffer<InType, OutType>::reset() {
  std::lock_guard<std::mutex> lock(_mutex);
  _head = _tail;
  _count = 0;
  _full = false;
  _cv.notify_one();
}