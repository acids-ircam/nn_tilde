#pragma once
#include "c74_min.h"
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
}

