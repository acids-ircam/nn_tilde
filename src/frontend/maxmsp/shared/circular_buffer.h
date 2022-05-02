#pragma once
#include <memory>

template <class in_type, class out_type> class circular_buffer {
public:
  circular_buffer();
  void initialize(size_t size);
  bool empty();
  bool full();
  void put(in_type *input_array, int N);
  void get(out_type *output_array, int N);
  void reset();

protected:
  std::unique_ptr<out_type[]> _buffer;
  size_t _max_size;

  int _head = 0;
  int _tail = 0;
  int _count = 0;
  bool _full = false;
};

template <class in_type, class out_type>
circular_buffer<in_type, out_type>::circular_buffer() {}

template <class in_type, class out_type>
void circular_buffer<in_type, out_type>::initialize(size_t size) {
  _buffer = std::make_unique<out_type[]>(size);
  _max_size = size;
}

template <class in_type, class out_type>
bool circular_buffer<in_type, out_type>::empty() {
  return (!_full && _head == _tail);
}

template <class in_type, class out_type>
bool circular_buffer<in_type, out_type>::full() {
  return _full;
}

template <class in_type, class out_type>
void circular_buffer<in_type, out_type>::put(in_type *input_array, int N) {
  if (!_max_size)
    return;

  while (N--) {
    _buffer[_head] = out_type(*(input_array++));
    _head = (_head + 1) % _max_size;
    if (_full)
      _tail = (_tail + 1) % _max_size;
    _full = _head == _tail;
  }
}

template <class in_type, class out_type>
void circular_buffer<in_type, out_type>::get(out_type *output_array, int N) {
  if (!_max_size)
    return;

  while (N--) {
    if (empty()) {
      *(output_array++) = out_type();
    } else {
      *(output_array++) = _buffer[_tail];
      _tail = (_tail + 1) % _max_size;
      _full = false;
    }
  }
}

template <class in_type, class out_type>
void circular_buffer<in_type, out_type>::reset() {
  _head = _tail;
  _count = 0;
  _full = false;
}