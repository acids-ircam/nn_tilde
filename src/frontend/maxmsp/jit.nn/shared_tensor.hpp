#pragma once

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>

#define X_DIM 256
#define Y_DIM 256
#define PLANE_COUNT 4

class Matrix {
public:
  float values[X_DIM * Y_DIM * PLANE_COUNT];
};

class ShmRemove {
public:
  ShmRemove(){};
  ShmRemove(std::string key) : m_key(key) {
    boost::interprocess::shared_memory_object::remove(m_key.c_str());
  };
  ~ShmRemove() {
    boost::interprocess::shared_memory_object::remove(m_key.c_str());
  };

private:
  std::string m_key;
};