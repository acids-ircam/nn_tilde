#pragma once
#include "c74_min.h"
#include <nlohmann/json.hpp>
#include "../../../backend/backend.h"

namespace nn_tools {

    namespace min = c74::min; 
    namespace max = c74::max;

    void append_to_dictionary(max::t_dictionary* d, max::t_symbol* key, max::t_dictionary* value) {
        auto parsed_value  = reinterpret_cast<max::t_object*>(value);
        max::dictionary_appenddictionary(d, key, parsed_value);
    }

    min::dict dict_from_model_info(const ModelInfo & info) {
        auto str = std::stringstream();
        auto new_dict = max::dictionary_new();

        // // append methods
        std::vector<std::string> method_names {};
        auto method_dict = max::dictionary_new();
        for (auto method_pair: info.method_properties) {
            method_names.push_back(method_pair.first);
            auto current_method_dict = max::dictionary_new();
            auto method_props = method_pair.second;
            max::dictionary_appendlong(current_method_dict, min::symbol("channels_in"), (long)method_props.channels_in);
            max::dictionary_appendlong(current_method_dict, min::symbol("channels_out"), (long)method_props.channels_out);
            max::dictionary_appendlong(current_method_dict, min::symbol("ratio_in"), (long)method_props.ratio_in);
            max::dictionary_appendlong(current_method_dict, min::symbol("ratio_out"), (long)method_props.ratio_out);
            append_to_dictionary(method_dict, min::symbol(method_pair.first), current_method_dict);
        }
        append_to_dictionary(new_dict, min::symbol("methods"), method_dict);

        // // append methods
        std::vector<std::string> attribute_names {};
        auto attribute_dict = max::dictionary_new();
        for (auto attribute_pair: info.attribute_properties) {
            attribute_names.push_back(attribute_pair.first);
            auto current_attribute_dict = max::dictionary_new();
            auto attr_types = attribute_pair.second.attribute_types;
            min::atoms attr_types_atoms(attr_types.size());
            std::transform(attr_types.begin(), attr_types.end(), attr_types_atoms.begin(),
                [](const std::string& str) {
                return min::atom(str); 
            });
            max::dictionary_appendatoms(current_attribute_dict, min::symbol("attribute_type"), attr_types_atoms.size(), attr_types_atoms.data());
            append_to_dictionary(attribute_dict, min::symbol(attribute_pair.first), current_attribute_dict);
        }
        append_to_dictionary(new_dict, min::symbol("attributes"), attribute_dict);
        auto out_dict = min::dict(new_dict);
        return out_dict;
    }
    
    void json_walk(max::t_dictionary* dict, nlohmann::json json) {
        for (auto& el : json.items()) {
            // std::cout << "key: " << el.key() << ", value:" << el.value() << '\n';
            min::symbol key = el.key(); 
            auto val = el.value();
            if (val.is_null()) {
            } else if ((json.is_boolean())||(json.is_number_integer())||(json.is_number_unsigned())) {
                max::dictionary_appendlong(dict, key, val.get<long>());
            } else if (val.is_number_float()) {
                max::dictionary_appendfloat(dict, key, val.get<float>());
            } else if (val.is_string()) {
                max::dictionary_appendsym(dict, key, min::symbol(val.get<std::string>())); 
            } else if (val.is_array()) {
                std::vector<min::atom> atoms; 
                for (const auto& v: val){
                    if ((v.is_boolean())||(v.is_number_integer())||(v.is_number_unsigned())) {
                        atoms.emplace_back(v.get<long>());
                    } else if (v.is_number_float()) {
                        atoms.emplace_back(v.get<float>());
                    } else if (v.is_string()) {
                        atoms.emplace_back(v.get<std::string>());
                    }
                }
                max::dictionary_appendatoms(dict, key, atoms.size(), atoms.data());
            } else if (val.is_object()) {
                auto sub_dict = max::dictionary_new();
                json_walk(sub_dict, val);
                append_to_dictionary(dict, key, sub_dict);
            } else {
                std::cerr << "Unknown type" << std::endl;
            }
        }
                
    }

    void fill_dict_with_json(min::dict* dict_to_fill, nlohmann::json json) {
        auto global_dict = max::dictionary_new();
        json_walk(global_dict, json);
        auto min_dict = min::dict(global_dict);
        dict_to_fill->copyunique(min_dict);
    }
}

