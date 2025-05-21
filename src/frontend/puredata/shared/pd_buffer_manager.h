#pragma once
#include "m_pd.h"
#include "../../../backend/backend.h"
#include "../../../shared/static_buffer.h"

#ifndef CHECK_BUFFERS_INTERVAL
#define CHECK_BUFFERS_INTERVAL 100
#endif


template <typename pd_class>
class PdBufferManager {

    Backend *m_backend; 
    pd_class *m_obj;
    t_clock *m_clock; 
    std::vector<std::string> m_buffer_attributes;
    std::unordered_map<std::string, std::string> m_array_to_buffer; 
    double m_sample_rate;
    
    public:
    bool m_monitor_arrays; 

    PdBufferManager(Backend *backend, pd_class* nn_obj): 
        m_backend(backend), m_obj(nn_obj)
    { }

    void init_buffer_list(Backend *backend = nullptr) {
        if (backend != nullptr) {
            m_backend = backend; 
        }
        // clear previous buffers
        m_buffer_attributes.clear();
        // init model buffers
        std::vector<std::string> model_buffers;
        try {
            model_buffers = m_backend->get_buffer_attributes();
        } catch (std::exception &e) {
            throw std::string("could not retrieve buffers from model. Caught error : ") + e.what();
        }
        // if (!model_buffers.size()) {
        //     std::cout << "no buffers found" << std::endl;
        // }
        // create buffer references for each of model buffers
        for (auto & element : model_buffers) {
            // std::cout << "adding buffer " << element << " to buffer manager.";
            m_buffer_attributes.push_back(element);
            post("nn~: %s", element.c_str());
        }
    } 

    void add_array_monitor(const std::string target_pd_buffer, const std::string buffer_name) {
        t_garray *garray;
        if (!(garray = (t_garray *)pd_findbyclass(gensym(target_pd_buffer.c_str()), garray_class))) {
            throw std::string("table " + buffer_name + " not found");
        }
        m_array_to_buffer[target_pd_buffer] = buffer_name; 
    }

    template <typename backend_type>
    StaticBuffer<typename backend_type::DataType> static_buffer_from_name(const std::string buffer_name) {
        t_garray *garray;
        if (!(garray = (t_garray *)pd_findbyclass(gensym(buffer_name.c_str()), garray_class))) {
            throw std::string("table " + buffer_name + " not found");
        }
        t_word *table_data;
        int table_size;
        if (!(garray_getfloatwords(garray, &table_size, &table_data))) {
            throw std::string("could not access table ") + std::string(buffer_name); 
        }
        auto buffer = StaticBuffer<typename backend_type::DataType>(1, table_size, (double)sys_getsr()); 
        for (int i(0); i < table_size; ++i) 
            buffer.put(static_cast<typename backend_type::DataType>((table_data + i)->w_float), 0, i); 
        return buffer; 
    }

    template <typename backend_type>
    void append_if_buffer_element(Backend::BufferMap &buffers, std::string target_pd_buffer, std::string attribute_name, int index) {
        if (m_backend->is_buffer_element_of_attribute(attribute_name, index)) {
            auto buffer_name = m_backend->get_buffer_name(attribute_name, index);
            buffers[buffer_name] = static_buffer_from_name<backend_type>(target_pd_buffer);
        } else {
            std::ostringstream error; 
            // error << std::to_string(index) << "th element of attribute " << attribute_name << "does not seem to be a buffer"; 
            throw error.str(); 
        }
    }

    auto get_buffer_attributes() { return m_buffer_attributes; }
    
};