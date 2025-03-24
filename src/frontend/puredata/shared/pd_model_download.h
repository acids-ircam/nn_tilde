#pragma once
#include "m_pd.h"
#include "../../../shared/model_download.h"


namespace fs = std::filesystem; 

template <typename pd_struct>
class PdModelDownloader: public ModelDownloader {
    const pd_struct* d_parent; 

public:
    PdModelDownloader(const pd_struct* parent): d_parent(parent) {}
    PdModelDownloader(const pd_struct* parent, const std::string path): d_parent(parent) { d_path = path; }

    void print_to_parent(const std::string &message, const std::string &canal) override {
        if (d_parent != nullptr) {
            if (canal == "cout") {
                post(message.c_str()); 
            } else if (canal == "cwarn") {
                post(message.c_str()); 
            } else if (canal == "cerr") {
                pd_error(d_parent, "nn~: %s", message.c_str()); 
            }
        }
    };

    void fill_dict(void* dict_to_fill) override {
        return; 
    }
};
