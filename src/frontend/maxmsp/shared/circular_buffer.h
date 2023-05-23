#pragma once
#include <memory>

template <class in_type, class out_type> class circular_buffer {
public:
  circular_buffer();
  void initialize(size_t size);
  bool empty();
  bool full();
  size_t size();
  size_t max_size();
  void put(in_type *input_array, int N);
  void put_interleave(in_type* input_array, int channels, int N);
  void get(out_type *output_array, int N);
  void get_interleave(out_type* output_array, int channels, int N);
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
size_t circular_buffer<in_type, out_type>::size() {
    if (full()) {
        return (int)_max_size;
    }
    else if (empty()) {
        return 0;
    }
    else if (_head >= _tail) {
        return _head - _tail;
    }
    else {
        return _max_size - (_head - _tail);
    }
}

template <class in_type, class out_type>
size_t circular_buffer<in_type, out_type>::max_size() {
    return _max_size;
}



template <class in_type, class out_type>
void circular_buffer<in_type, out_type>::put(in_type *input_array, int N) {
  if (!_max_size)
    return;

  while (N--) {
    // std::cout << "input: " << input_array[0] << ", output: " << out_type(*(input_array)) << std::endl;
    _buffer[_head] = out_type(*(input_array++));
    _head = (_head + 1) % _max_size;
    if (_full)
      _tail = (_tail + 1) % _max_size;
    _full = _head == _tail;
  }
}

// Put from an interleaved format
template <class in_type, class out_type>
void circular_buffer<in_type, out_type>::put_interleave(in_type* input_array, int channels, int N) {
    if (!_max_size)
        return;

    while (N--) {
        // std::cout << "input: " << input_array[0] << ", output: " << out_type(*(input_array)) << std::endl;
        _buffer[_head] = out_type(*(input_array));
        input_array += channels;
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

// get to an interleaved format
template <class in_type, class out_type>
void circular_buffer<in_type, out_type>::get_interleave(out_type* output_array, int channels, int N) {
    if (!_max_size)
        return;

    while (N--) {
        if (empty()) {
            *(output_array) = out_type();
        }
        else {
            *(output_array) = _buffer[_tail];

            _tail = (_tail + 1) % _max_size;
            _full = false;
        }
        output_array += channels;
        }
}


template <class in_type, class out_type>
void circular_buffer<in_type, out_type>::reset() {
  _head = _tail;
  _count = 0;
  _full = false;
}
