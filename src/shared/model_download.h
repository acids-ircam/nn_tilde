#pragma once

#include <cstdio>
#include <filesystem>
#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <thread>

#ifndef MAX_DOWNLOADS
#define MAX_DOWNLOADS 2
#endif

namespace fs = std::filesystem;
using json = nlohmann::json;
using DownloadTask = std::function<void()>;


// Callback function for handling the downloaded data
size_t JSONWriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    size_t totalSize = size * nmemb;
    userp->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

size_t ModelWriteCallback(void* ptr, size_t size, size_t nmemb, FILE* stream) {
    return fwrite(ptr, size, nmemb, stream);
}

bool is_file_empty(const fs::path filePath) {
    try {
        return fs::is_regular_file(filePath) && fs::file_size(filePath) == 0;
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
        return false;
    }
}



class ModelDownloader {

protected:
    fs::path d_path, d_cert_path;
    bool _is_ready = false;
    static bool create_path(const fs::path& path);
    // api root
    std::string _api_root;
    // available models callbacks
    std::string get_string_from_api_callback(std::string &adress);
    json d_available_models;
    // downloading attributes
    std::vector<std::thread> d_threads;
    std::queue<DownloadTask> tasks;
    std::mutex mutex;
    std::condition_variable condition; 
    bool stop;
    // helpers


public: 
    ModelDownloader() = default; 
    ModelDownloader(fs::path download_location); 
    ~ModelDownloader(); 
    int init_downloader(bool force_refresh = false);
    void init_threads();
    bool is_ready();
    bool has_valid_certificate();
    bool has_model(const std::string &model_card);
    std::string get_api_root();
    bool update_available_models(); 
    std::vector<std::string> get_available_models(); 
    void download(const std::string &model_name, const std::string &custom_name = ""); 
    void remove(const std::string &model_name); 
    void enqueue_download_task(DownloadTask task);
    void print_available_models();
    fs::path target_path_from_model(const std::string model_name, const std::string custom_name = "");
    fs::path get_certificate_path() { return d_cert_path; }
    void reload();
    void worker(); 

    virtual void fill_dict(void* dict_to_fill) = 0;
    virtual void print_to_parent(const std::string &message, const std::string &canal) = 0;
    virtual fs::path cert_path_from_path(fs::path path) = 0;

    std::string string_id() {
        std::stringstream str_id; 
        str_id << this; 
        return str_id.str();
    }

    void find_and_replace_char(std::string& str, char oldChar, char newChar) {
        for (size_t pos = 0; (pos = str.find(oldChar, pos)) != std::string::npos; pos++) {
            str[pos] = newChar;
        }
    }
};

ModelDownloader::ModelDownloader(fs::path download_location): d_path(download_location / "..") {}

ModelDownloader::~ModelDownloader() {
    {
        std::unique_lock<std::mutex> lock(mutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread &thread : d_threads) {
        thread.join();
    }
}

void ModelDownloader::init_threads() {
    for (size_t i(0); i < MAX_DOWNLOADS; ++i) {
        d_threads.emplace_back(&ModelDownloader::worker, this);
    }
}

bool ModelDownloader::has_valid_certificate() {
    if (d_cert_path == "") {
        return false;
    } else if (!fs::exists(d_cert_path)) {
        #if defined(_WIN32) || defined(_WIN64)
            std::string error_message = "Could not find certificate at " + d_cert_path.string();
        #elif defined(__APPLE__) || defined(__MACH__)
            std::string error_message = "Could not find certificate in external bundle";
        #elif defined(__linux__)
            std::string error_message = "Could not find certificate at " + d_cert_path.string() + "; did you install ca-certificates?";
        #else
            std::string error_message ="Could not find certificate at " + d_cert_path.string() + "; wrong compilation platform";
        #endif
        
        print_to_parent(error_message, "cwarn");
        return false;
    }
    return true;
}

int ModelDownloader::init_downloader(bool force_refresh) {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    bool is_path_ok = create_path(d_path);
    bool is_certificate_ok = has_valid_certificate(); 
    bool is_model_list_available = update_available_models();
    init_threads();
    _is_ready = (is_path_ok && is_model_list_available);
    return _is_ready;
}

bool ModelDownloader::is_ready() {
    if (!_is_ready) {
        return init_downloader();
    } else {
        return true;
    }
}

bool ModelDownloader::update_available_models() {
    _api_root = get_api_root();
    auto url = _api_root + "available_models";
    auto model_as_string = get_string_from_api_callback(url);
    d_available_models = nlohmann::json::parse(model_as_string);
    return true;
}

std::string ModelDownloader::get_string_from_api_callback(std::string &address) {
    CURL* curl; 
    CURLcode res; 
    std::string readBuffer; 

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, address.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, JSONWriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_CAINFO, d_cert_path.string().c_str());
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            throw std::string("could not fetch available models from API. Code from API: ") + std::to_string(res);
        }
        // Clean up
        curl_easy_cleanup(curl);
    } else {
        throw std::string("curl is not available ; cannot download models");
    }
    return readBuffer; 
}

void ModelDownloader::reload() {
    update_available_models(); 
}

std::string ModelDownloader::get_api_root() {
unsigned char b[] = {104, 116, 116, 112, 115, 58,  47,  47, 112, 108, 97,
                    121, 46,  102, 111, 114, 117, 109, 46, 105, 114, 99,
                    97,  109, 46,  102, 114, 47,  114, 97, 118, 101, 45,
                    118, 115, 116, 45,  97,  112, 105, 47};
char c[sizeof(b) + 1];
std::memcpy(c, b, sizeof(b));
c[sizeof(b)] = '\0';
return std::string(c);
}

bool ModelDownloader::create_path(const fs::path& path) {
    if (fs::exists(path)) {
        return true;
    } else {
        if (fs::create_directories(path)) {
            return true;
        } else {
            return false;
        }
    }
}

std::vector<std::string> ModelDownloader::get_available_models() {
    if (!_is_ready) {
        throw std::string("model downloader has not been initialised, or did not manage to fetch available content");
    }
    auto models_keys = std::vector<std::string>();
    for (const auto& source : d_available_models.items()) {
        auto current_source = source.key(); 
        for (const auto& model: source.value().items()) {
            auto current_model = model.key(); 
            for (const auto& model_name: model.value().items()) {
                std::stringstream model_card(""); 
                model_card << current_source << "/" << current_model << "/" << model_name.key(); 
                models_keys.push_back(model_card.str()); 
            }
        }
    }
    return models_keys;
}

void ModelDownloader::print_available_models() {
    auto available_models = get_available_models();
    for (const auto& model_name: available_models) {
        print_to_parent(model_name, "cout");
    }
}


void ModelDownloader::worker() {
    while (true) {
        DownloadTask task;
        {
            std::unique_lock<std::mutex> lock(mutex);
            condition.wait(lock, [this]() { return stop || !tasks.empty(); });

            if (stop && tasks.empty()) {
                return;
            }
            task = std::move(tasks.front());
            tasks.pop();
        }
        task();
    }
}

void ModelDownloader::enqueue_download_task(DownloadTask task) {
    {
        std::unique_lock<std::mutex> lock(mutex);
        tasks.push(task);
    }
    condition.notify_one();   
}

std::vector<std::string> split_model_card(std::string model_name) {
    auto ss = std::stringstream(model_name);
    std::string segment;
    std::vector<std::string> segments;
    while (std::getline(ss, segment, '/')) {
        segments.push_back(segment);
    }
    return segments;
}

fs::path ModelDownloader::target_path_from_model(const std::string model_name, const std::string custom_name) {
    std::string name_to_write;
    if (custom_name == "") {
        auto segments = split_model_card(model_name);
        name_to_write = segments[segments.size() - 1];
    } else {
        name_to_write = custom_name; 
    }
    return d_path / (name_to_write + ".ts");
}

std::string lock_path_from_target(std::string target_path, std::string model_name) {
    fs::path target_path_fs = target_path;
    std::stringstream lock_name("."); 
    for (auto p: split_model_card(model_name)) {
        lock_name << p << '_' ; 
    }
    lock_name << "lock";
    std::string lock_path = fs::absolute(target_path_fs.parent_path() / lock_name.str()).string(); 
    return lock_path;
}

void download_thread(ModelDownloader *parent, std::string model_name, std::string target_path, int dl_idx)
{
    parent->print_to_parent("downloading model " + model_name + "...", "cwarn");
    // create lock file, to not download two times the same model
    std::string lock_path = lock_path_from_target(target_path, model_name);
    FILE* lock_file = fopen(lock_path.c_str(), "w");
    fputc('l', lock_file);
    if (!lock_file) {
        parent->print_to_parent(std::string("could not open file for writing : ") + target_path, "cerr"); 
        return;
    }

    try {
        CURL* curl;
        FILE* file;
        CURLcode res;

        curl = curl_easy_init();
        std::string url = (parent->get_api_root()) + "download_model?model=" + model_name; 
        if (curl) {
            file = fopen(target_path.c_str(), "wb");
            if (!file) {
                curl_easy_cleanup(curl);
                parent->print_to_parent(std::string("could not open file for writing : ") + target_path, "cerr"); 
                fclose(lock_file);
                fs::remove(lock_path);
                return;
            }

            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ModelWriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
            curl_easy_setopt(curl, CURLOPT_CAINFO, parent->get_certificate_path().string().c_str());
            
            // Perform the request
            res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                parent->print_to_parent("error during download : " + std::string(curl_easy_strerror(res)), "cerr");
                fclose(file);
                fclose(lock_file);
                fs::remove(lock_path);
                curl_easy_cleanup(curl);
                return;
            } 

            // Clean up
            fclose(file);
            curl_easy_cleanup(curl);
            if (is_file_empty(fs::path(target_path))) {
                parent->print_to_parent("failed to download  " + model_name, "cerr");
                fs::remove(target_path);
            } else {
                parent->print_to_parent("model " + model_name + " downloaded at " + target_path, "cwarn");
            }
            fclose(lock_file);
            fs::remove(lock_path);
            return; 
        }
    } catch (...) {
        parent->print_to_parent("failed to download  " + model_name, "cerr");
        fclose(lock_file);
        fs::remove(lock_path);
        return ; 
    }
    
}

void ModelDownloader::remove(const std::string &model_name) {
    auto target_path = target_path_from_model(model_name);
    if (fs::exists(target_path)) {
        fs::remove(target_path);
        print_to_parent("model " + model_name + " deleted.", "cwarn");
    } else {
        print_to_parent("could not find model " + model_name , "cerr");
    }
}

bool ModelDownloader::has_model(const std::string &model_name) {
    auto parts = split_model_card(model_name);
    auto models_json = d_available_models; 
    for (auto p: parts) {
        if (models_json.contains(p)) {
            models_json = models_json[p];
        } else {
            return false;
        } 
    }
    return true; 
}

void ModelDownloader::download(const std::string &model_name, const std::string &custom_name) {
    if (!_is_ready) {
        throw std::string("model downloader has not been initialised, or did not manage to fetch available content");
    }
    if (!has_model(model_name)) {
        throw std::string("model name " + model_name + " not available.");
    }
    auto target_path = target_path_from_model(model_name, custom_name);
    if (std::filesystem::exists(target_path)) {
        if (!is_file_empty(target_path)) {
            print_to_parent("model " + target_path.string() + " seems to be already downloaded.", "cwarn");
            return;
        } else {
            std::filesystem::remove(target_path);
        }
    }
    if (std::filesystem::exists(lock_path_from_target(target_path.string(), model_name))) {
        print_to_parent("model " + model_name + " is already downloading.", "cwarn");
        return; 
    }
    int download_id = d_threads.size();
    enqueue_download_task([this, model_name, target_path, download_id]() { download_thread(this, model_name, target_path.string(), download_id); });
}
