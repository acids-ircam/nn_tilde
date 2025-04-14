#pragma once
#include "../../../backend/backend.h"
#include "../../../shared/static_buffer.h"
#include "c74_min.h"

class BufferManager {

    std::vector<std::unique_ptr<c74::min::buffer_reference>> m_max_buffers; 
    std::vector<std::string> buffer_attributes;
    bool buffer_track = false;

public: 

    BufferManager();

    void init_buffer_list(Backend *backend, c74::min::object_base* object);
    void link_attribute_to_buffer(std::string buffer_name, c74::min::symbol target_max_buffer);
    void append_if_buffer_element(Backend *model, Backend::BufferMap &buffers, c74::min::symbol target_max_buffer, std::string attribute_name, int index);
    static int get_buffer_index(std::vector<std::string> buffer_attributes, std::string buffer_name);

    void set_buffer_tracking(bool buffer_tracking) { buffer_track = buffer_tracking; }

    int bind_buffer_attribute(Backend* backend, std::string element, c74::min::object_base* object);
    int unbind_buffer_attribute(Backend* backend, std::string element, c74::min::object_base* object);
    int modify_buffer_attribute(Backend* backend, std::string element, c74::min::object_base* object);

    template <typename data_type>
    StaticBuffer<data_type> static_buffer_from_name(std::string buffer_name);
    template <typename data_type>
    static StaticBuffer<data_type> static_buffer_from_max_buffer(c74::min::buffer_reference* max_buffer);

    c74::min::function get_notification_callback(std::string element, c74::min::object_base* object, Backend* backend);

    using iterator = std::vector<std::unique_ptr<c74::min::buffer_reference>>::iterator;
    auto begin() { return m_max_buffers.begin(); }
    auto end() { return m_max_buffers.end(); }

    std::string string_id() {
      std::stringstream str_id; 
      str_id << this; 
      return str_id.str();
    }
    
};


BufferManager::BufferManager() {
}


int BufferManager::get_buffer_index(std::vector<std::string> buffer_attributes, std::string buffer_name) {
  int buffer_idx = -1;
  for (int i = 0; i < buffer_attributes.size(); i++) {
    if (buffer_name == buffer_attributes[i]) {
      buffer_idx = i;
      break;
    }
  }
  return buffer_idx;
}

void BufferManager::link_attribute_to_buffer(std::string buffer_name, c74::min::symbol target_max_buffer) {
  int buffer_index = get_buffer_index(buffer_attributes, buffer_name);
  if (buffer_index > -1) {
    m_max_buffers[buffer_index].get()->set(target_max_buffer);
  } else {
    throw "could not link" + buffer_name + "to max buffer" + target_max_buffer.c_str();
  }
}

void BufferManager::append_if_buffer_element(Backend *model, Backend::BufferMap &buffers, c74::min::symbol target_max_buffer, std::string attribute_name, int index)
{
    if (model->is_buffer_element_of_attribute(attribute_name, index)) {
        auto buffer_name = model->get_buffer_name(attribute_name, index);
        link_attribute_to_buffer(buffer_name, target_max_buffer);
        buffers[buffer_name] = static_buffer_from_name<float>(buffer_name);
    }
}
template <typename data_type>
StaticBuffer<data_type> BufferManager::static_buffer_from_name(std::string buffer_name) {

  int buffer_idx = get_buffer_index(buffer_attributes, buffer_name);
  if ((buffer_idx  == -1) || (buffer_idx > m_max_buffers.size())) {
    throw std::string("could not retrieve buffer from name") + buffer_name;
  }
  c74::min::buffer_reference *buffer_ref = m_max_buffers[buffer_idx].get();
  try {
    return static_buffer_from_max_buffer<data_type>(buffer_ref);
  } catch (std::string &e) {
    // cerr << "could not link buffer " << buffer_name;
    // if (buffer_ref->name() != symbol()) {
    //   cerr << "; buffer name " << buffer_ref->name() << " seems to be invalid.";
    // } 
    // cerr << endl;
    throw e;
  }
}

template <typename data_type>
StaticBuffer<data_type> BufferManager::static_buffer_from_max_buffer(c74::min::buffer_reference* max_buffer) {
    c74::min::buffer_lock<false> b(*max_buffer);
    const size_t n_channels = b.channel_count();
    const size_t n_samples = b.frame_count();
    const double sample_rate = b.samplerate();
    if (b.valid()) {
        auto data = StaticBuffer<data_type>(n_channels, n_samples, sample_rate);
        for (auto c = 0; c < n_channels; c++) {
            for (auto t = 0; t < n_samples; t++) {
                data.put(b.lookup(t, c), c, t);
            }
        }
        return data;
    } else {
        throw "given max buffer is invalid.";
    }
};


int BufferManager::bind_buffer_attribute(Backend *backend, std::string element, c74::min::object_base* object) {
if (this->buffer_track) {
    std::string method = "print";
    // c74::min::atoms args = {string_id(), "cout", element + " modified"};
    // object->try_call(method, args);
    int buffer_idx = get_buffer_index(buffer_attributes, element);
    if (buffer_idx == -1) { return -1; }
    auto buffer_name = m_max_buffers[buffer_idx].get()->name();
    m_max_buffers[buffer_idx].release();
    m_max_buffers[buffer_idx] = std::make_unique<c74::min::buffer_reference>(object, get_notification_callback(element, object, backend), false);
    m_max_buffers[buffer_idx].get()->set(buffer_name);

    // if (current_buffer_ref != bufferRef) {
    //   m_max_buffers[buffer_idx].release();
    //   m_max_buffers[buffer_idx] = std::unique_ptr<c74::min::buffer_reference>(bufferRef);
    // }
    try {
      auto buffer = static_buffer_from_max_buffer<float>(m_max_buffers[buffer_idx].get());
      int res = backend->update_buffer(element, buffer);
      return res;
    } catch (...) {
      c74::min::atoms args = {string_id(), "cerr", "failed to bind buffer " + element};
      object->try_call(method, args);
    }
  }
  return -1;
}


int BufferManager::unbind_buffer_attribute(Backend *backend, std::string element, c74::min::object_base* object) {
  if (this->buffer_track) {
    std::string method = "print";
    // c74::min::atoms args = {string_id(), "cout", element + " unbounded"};
    // object->try_call(method, args);
    // get buffer
    int buffer_idx = get_buffer_index(buffer_attributes, element);
    if (buffer_idx == -1) { return -1; }
    auto current_buffer_ref = m_max_buffers[buffer_idx].get();
    // if (current_buffer_ref != bufferRef) {
    //   m_max_buffers[buffer_idx].release();
    //   m_max_buffers[buffer_idx] = std::unique_ptr<c74::min::buffer_reference>(bufferRef);
    // }
    try {
      int res = backend->reset_buffer(element);
      return res;
    } catch (...) {
      c74::min::atoms args = {string_id(), "cerr", "failed to unbind buffer " + element};
      object->try_call(method, args);
    }
  }
  return -1;
}

int BufferManager::modify_buffer_attribute(Backend *backend, std::string element, c74::min::object_base* object) {
  if (this->buffer_track) {
    std::string method = "print";
    // c74::min::atoms args = {string_id(), "cout", element + " modified"};
    // object->try_call(method, args);
    int buffer_idx = get_buffer_index(buffer_attributes, element);
    if (buffer_idx == -1) { return -1; }
    auto buffer_name = m_max_buffers[buffer_idx].get()->name();
    m_max_buffers[buffer_idx].release();
    m_max_buffers[buffer_idx] = std::make_unique<c74::min::buffer_reference>(object, get_notification_callback(element, object, backend), false);
    m_max_buffers[buffer_idx].get()->set(buffer_name);

    // if (current_buffer_ref != bufferRef) {
    //   m_max_buffers[buffer_idx].release();
    //   m_max_buffers[buffer_idx] = std::unique_ptr<c74::min::buffer_reference>(bufferRef);
    // }
    try {
      auto buffer = static_buffer_from_max_buffer<float>(m_max_buffers[buffer_idx].get());
      int res = backend->update_buffer(element, buffer);
      return res;
    } catch (...) {
      c74::min::atoms args = {string_id(), "cerr", "failed to bind buffer " + element};
      object->try_call(method, args);
    }
  }
  return -1;
}

c74::min::function BufferManager::get_notification_callback(std::string element, c74::min::object_base* object, Backend* backend) {
  c74::min::function bufferCallback = 
        [this, object, element, backend](const c74::min::atoms& args, const int inlet) -> c74::min::atoms {
            int out;
            c74::min::symbol buffer_message = args[0];
            // cout << "callback" << buffer_message << "for element " << element << "called" << endl;
            if (buffer_message == "binding") {
              out = this->bind_buffer_attribute(backend, element, object);
            } else if (buffer_message == "unbinding") {
              out = this->unbind_buffer_attribute(backend, element, object);
            } else if (buffer_message == "modified") {
              out = this->modify_buffer_attribute(backend, element, object);
            } else {
            //   cerr << "got unsupported buffer notification " << args << "for element" << element << endl;
            }
            if (out != 0) {
            //   cerr << "problem setting buffer " << element << endl;
            }
            return std::vector<c74::min::atom>();
        };
    return bufferCallback;
}

void BufferManager::init_buffer_list(Backend *backend, c74::min::object_base* object) {
    // clear previous buffers
    m_max_buffers.clear();
    buffer_attributes.clear();
    // init model buffers
    std::vector<std::string> model_buffers;
    try {
      model_buffers = backend->get_buffer_attributes();
    } catch (std::exception &e) {
        throw std::string("could not retrieve buffers from model. Caught error : ") + e.what();
    }
    // create buffer references for each of model buffers
    for (auto & element : model_buffers) {
      std::unique_ptr<c74::min::buffer_reference> bufferRef;
      
      // m_max_buffer_callbacks.push_back(bufferCallback);
      // add buffer id to buffer_attributes
      buffer_attributes.push_back(element);
      // add pointer to buffer_reference
      m_max_buffers.push_back(std::make_unique<c74::min::buffer_reference>(object, get_notification_callback(element, object, backend), false));
    }
}


void fill_with_zero(c74::min::audio_bundle output) {
  for (int c(0); c < output.channel_count(); c++) {
    auto out = output.samples(c);
    for (int i(0); i < output.frame_count(); i++) {
      out[i] = 0.;
    }
  }
}
