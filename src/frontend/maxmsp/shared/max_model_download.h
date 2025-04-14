#pragma once

#include <cstdio>
#include <filesystem>
#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include "c74_min.h"
#include <thread>
#include "dict_utils.h"
#include "../../../shared/model_download.h"



#ifndef MAX_DOWNLOADS
#define MAX_DOWNLOADS 2
#endif



namespace max = c74::max;
namespace min = c74::min;


class MaxModelDownloader: public ModelDownloader {
    c74::min::object_base* d_parent;

public:
    MaxModelDownloader(c74::min::object_base* obj);
    MaxModelDownloader(c74::min::object_base* obj, std::string external_name); 
    MaxModelDownloader(c74::min::object_base* obj, fs::path download_location); 

    void fill_dict(void* dict_to_fill);
    void print_to_parent(const std::string &message, const std::string &canal);
    fs::path cert_path_from_path(fs::path path) {
        #if defined(_WIN32) || defined(_WIN64)
            std::string perm_path = (path / ".." / ".." / "support" / "cacert.pem").string();
            find_and_replace_char(perm_path, '/', '\\');
        #elif defined(__APPLE__) || defined(__MACH__)
            std::string perm_path = path / "Contents" / "MacOS" / "cert.pem";
        #else
            std::string perm_path = (path / "..").string();
        #endif
        return perm_path;
    }
};


MaxModelDownloader::MaxModelDownloader(c74::min::object_base* obj): d_parent(obj) {
    // d_path = d_path / ".." / "nn_tilde" / "models";
    min::path path = min::path("nn~", min::path::filetype::external); 
    std::string path_str = path;
    if (path) {
        d_cert_path = cert_path_from_path(fs::path(path_str));
        d_path = fs::absolute(fs::path(path_str) / "..");
    }
}

MaxModelDownloader::MaxModelDownloader(c74::min::object_base* obj, std::string external_name): d_parent(obj) {
    min::path path = min::path(external_name, min::path::filetype::external);
    std::string path_str = path;
    fs::path fs_path(path_str);
    if (path) {
        d_cert_path = cert_path_from_path(fs::path(path_str));
        d_path = fs::absolute(fs::path(path_str) / "..");
    }
}

MaxModelDownloader::MaxModelDownloader(c74::min::object_base* obj, fs::path download_location): ModelDownloader(download_location), d_parent(obj) {
}


void MaxModelDownloader::print_to_parent(const std::string &message, const std::string &canal) {
    std::string method = "print";
    min::atoms args = {string_id(), canal, message};
    d_parent->try_call(method, args);
}

void MaxModelDownloader::fill_dict(void* dict_to_fill) {
    if (dict_to_fill == nullptr) {
        throw "dict is empty";
    }
    min::dict* max_dict = static_cast<min::dict*>(dict_to_fill);
    if (!max_dict->valid()) {
        throw "dict is invalid";
    }
    auto json_models = d_available_models; 
    nn_tools::fill_dict_with_json(max_dict, json_models);     
}