#include "../../../backend/backend.h"
#include "../shared/max_model_download.h"
#include "../shared/buffer_tools.h"
#include "../shared/dict_utils.h"
#include "c74_min.h"
#include <chrono>
#include <semaphore>
#include <string>
#include <thread>
#include <vector>

#ifndef VERSION
#define VERSION "UNDEFINED"
#endif

using namespace c74::min;

class nn_info : public object<nn_info>, public vector_operator<> {
public:
  MIN_DESCRIPTION{"Fetching information from deep learning models"};
  MIN_TAGS{"audio, deep learning, ai"};
  MIN_AUTHOR{"Axel Chemla--Romeu-Santos"};

  nn_info(const atoms &args = {});
  ~nn_info();

  // INLETS OUTLETS
  std::vector<std::unique_ptr<inlet<>>> m_inlets;
  std::vector<std::unique_ptr<outlet<>>> m_outlets;

  // BACKEND RELATED MEMBERS
  ModelInfo m_model_info; 
  std::string m_path;
  bool has_model = false;
  std::unique_ptr<dict> m_model_dict;
  std::unique_ptr<dict> m_available_models_dict;
  std::unique_ptr<MaxModelDownloader> m_downloader;
  bool is_valid_print_key(std::string string);

  // ONLY FOR DOCUMENTATION
  argument<symbol> path_arg{this, "model path",
                            "Absolute path to the pretrained model."};

  // FUNCTION
  void set_model_path(const std::string& model_path);
  void dump_path();
  void dump_methods();
  void dump_attributes();
  void dump_method_parameters(const std::string& method);
  void dump_attribute_parameters(const std::string& method);
  void dump_dictionary();
  void dump_object();
  void dump_downloadable_models(); 
  void scan_model(const path path);
  void update_dictionary(); 
  void bind_dictionary(const std::string &dict_name);
  void operator()(audio_bundle input, audio_bundle output) {}
  void perform(audio_bundle input, audio_bundle output) {}

  // BOOT STAMP
  message<> maxclass_setup{
      this, "maxclass_setup",
      [this](const c74::min::atoms &args, const int inlet) -> c74::min::atoms {
        cout << "nn.info " << VERSION << " - torch " << TORCH_VERSION
             << " - 2025 - Antoine Caillon & Axel Chemla--Romeu-Santos" << endl;
        cout << "visit https://forum.ircam.fr" << endl;
        return {};
      }};

  // ATTRIBUTES
  attribute<symbol> dict_attribute{
    this, "dict", symbol(""), 
    description{"bind information to target dictionary"}, 
    setter{ MIN_FUNCTION {
      std::string dictionary_name = args[0];
      this->bind_dictionary(dictionary_name);
      return {};
    }}
  };


  message<> print {
    this, "print", 
    MIN_FUNCTION {
      bool is_valid = is_valid_print_key(args[0]);
      if (!is_valid) {
        return {};
      }
      if (args[1] == "cout") {
        cout << args[2] << endl;
      } else if (args[1] == "cerr") {
        cerr << args[2] << endl;
      } else if (args[1] == "cwarn") {
        cwarn << args[2] << endl;
      }
      return {};
    }
  };

  // MESSAGES
  message<> bang_callback{
      this, "bang", 
        description{"dumps every model information available"},
        MIN_FUNCTION {
          this->dump_object();  
          return {};
        }};

  // MESSAGES
  message<> dump_callback{
      this, "dump", 
        description{"dumps every model information available"},
        MIN_FUNCTION {
          this->dump_object();  
          return {};
        }};

  message<> set_model_callback{
      this, "set", 
        description{"set model path"},
        MIN_FUNCTION {
          if (args.size() == 0) {
            cerr << "set message needs a path to a valid model." << endl;
          }
          std::string model_path = args[0];
          set_model_path(model_path);  
          return {};
        }};

  message<> get_path_callback {
    this, "path", 
      description{"dumps current model path"},
      MIN_FUNCTION {
        this->dump_path();  
        return {};
      }};

  message<> get_methods_callback {
    this, "methods", 
    description{"get available methods for provided path"}, 
    MIN_FUNCTION {
      this->dump_methods(); 
      return {};
    }};
  
  message<> get_attributes_callback {
    this, "attributes", 
    description{"provides settable attributes for provided path"}, 
    MIN_FUNCTION{
      this->dump_attributes(); 
      return {};
    }
  };

  message<> get_params_callback {
    this, "parameters", 
    description{"provides processing parameters for the given method"}, 
    MIN_FUNCTION{
      if (args.size() == 0) {
        cerr << "parameters takes a valid method as first argument." << endl;
      }
      std::string method_or_attribute = args[0];
      if (m_model_info.attribute_properties.contains(method_or_attribute)) {
        this->dump_attribute_parameters(method_or_attribute);
      } else if (m_model_info.method_properties.contains(method_or_attribute)) {
        this->dump_method_parameters(method_or_attribute);
      } else {
        cerr << method_or_attribute << " not found in model" << endl;
      }
      return {};
    }
  };

  message<> get_dict_callback {
    this, "dump_dict", 
    description{"dump information as dictionary"}, 
    MIN_FUNCTION{
      this->dump_dictionary();
      return {};
    }
  };

  message<> get_models_callback {
    this, "get_available_models", 
    description{"dump available models as a dictionary, with additional informations"}, 
    MIN_FUNCTION{
      this->dump_downloadable_models();
      return {};
    }
  };

  message <> download_models {
    this, "download", 
    description{"download a model from IRCAM Forum API"}, 
    MIN_FUNCTION {
      std::string model_card, optional_name;
      if (args.size() == 0) {
        cerr << "please provide a model card (print downloadable models with get_available_models messages)" << endl;
      } else if (args.size() == 1) {
        min::symbol model_card_s = args[0];
        model_card = std::string(model_card_s.c_str());
        optional_name = "";
      } else {
        min::symbol model_card_s = args[0];
        min::symbol optional_name_s = args[1]; 
        model_card = std::string(model_card_s.c_str());
        optional_name = std::string(optional_name_s.c_str()); 
      }
      try {
        if (this->m_downloader.get()->is_ready())
          this->m_downloader.get()->download(model_card, optional_name);
      } catch (std::string &e) {
        cerr << e << endl;
      }
      return {}; 
    }
  };

  message <> delete_models {
    this, "delete", 
    description{"delete a model downloaded from IRCAM Forum API"}, 
    MIN_FUNCTION {
      if (args.size() == 0) {
        cerr << "please provide a model to delete" << endl;
      }
      std::string model_card = args[0];
      try {
        if (this->m_downloader.get()->is_ready())
          this->m_downloader.get()->remove(model_card);
      } catch (std::string &e) {
        cerr << e << endl;
      }
      return {}; 
    }};
};


nn_info::nn_info(const atoms &args)
{
  // make inlets
  m_inlets.push_back(std::make_unique<inlet<>>(this, "input for nn.info"));
  // make outlets
  m_outlets.push_back(
    std::make_unique<outlet<>>(this, "model path", "symbol")
  );
  m_outlets.push_back(
    std::make_unique<outlet<>>(this, "available methods")
  );
  m_outlets.push_back(
    std::make_unique<outlet<>>(this, "available attributes")
  );
  m_outlets.push_back(
    std::make_unique<outlet<>>(this, "processing parameters")
  );
  m_outlets.push_back(
    std::make_unique<outlet<>>(this, "dict output", "dictionary")
  );
  m_outlets.push_back(
    std::make_unique<outlet<>>(this, "available models for download", "dictionary")
  );

  // import informations from model
  if (args.size() > 0) { // ONE ARGUMENT IS GIVEN
    auto model_path = std::string(args[0]);
    if (model_path.substr(model_path.length() - 3) != ".ts")
      model_path = model_path + ".ts";
    auto current_path = path(model_path);
    try {
      scan_model(current_path);
    } catch (std::string& errorstr) {
      cerr << errorstr << endl; 
      error();
    }
  }

  try {
    m_downloader = std::make_unique<MaxModelDownloader>(this, std::string("nn.info")); 
  } catch (...) {
    cwarn << "could not initialise model downloader" << endl;
  }
}

bool nn_info::is_valid_print_key(std::string id_string) {
  if (id_string == m_downloader.get()->string_id()) {
    return true;
  }
  return false;
}


void nn_info::update_dictionary() {
  if ((m_model_dict.get() == nullptr) || (!has_model)) {
    return;
  }
  auto dict_ref = m_model_dict.get();
  if (!dict_ref->valid()) {
    cwarn << "problem with dictionary : " << dict_ref->name() << " seems to be unvalid" << endl;
    return;
  }
  dict_ref->clear();
  auto model_dict = nn_tools::dict_from_model_info(m_model_info);
  dict_ref->copyunique(model_dict);
  return;
}

void nn_info::bind_dictionary(const std::string& dict_name) {
    if (m_model_dict.get() != nullptr) {
      m_model_dict.release();
    }
  if (dict_name == "") {
    m_model_dict = std::make_unique<dict>(symbol(true));
  } else {
    m_model_dict = std::make_unique<dict>(symbol(dict_name));
  }
  update_dictionary();
}

void nn_info::scan_model(path path) {
  std::string model_path = path;
  auto m_model = Backend();
  if (m_model.load(model_path, samplerate())) {
    // cerr << "error loading path " << model_path << endl;
    throw "error loading path " + model_path;
    return;
  }
  // parse things
  try {
    m_model_info = m_model.get_model_info();
  } catch (std::string &error) {
    throw error; 
  }
  has_model = true; 
  m_path = model_path;
  update_dictionary();
}

void nn_info::set_model_path(const std::string& model_path) {
  auto model_path_checked = model_path;
  try {
    if (model_path.substr(model_path.length() - 3) != ".ts")
      model_path_checked = model_path_checked + ".ts";
    auto current_path = path(model_path_checked);
    scan_model(current_path);
  } catch (std::string& stringerr) {
    cerr << stringerr << endl;
  }
}

void nn_info::dump_path() {
  // Iterating over the keys
  auto outlet = m_outlets[0].get();
  if (!has_model) {
    outlet->send(symbol("none"));
  } else {
    std::string path = m_path;
    outlet->send(symbol(path));
  }
  
}

void nn_info::dump_methods() {
  if (!has_model) {
    cerr << "please set a model before" << endl; 
    return;
  }
  // Iterating over the keys
  auto outlet = m_outlets[1].get();
  for (const auto& pair : m_model_info.method_properties) {
    auto key = pair.first;
    outlet->send({symbol("method"), symbol(key)});
  }
}

void nn_info::dump_attributes() {
  if (!has_model) {
    cerr << "please set a model before" << endl; 
    return;
  }
  // Iterating over the keys
  auto outlet = m_outlets[2].get();
  for (const auto& pair : m_model_info.attribute_properties) {
    auto key = pair.first;
    outlet->send({symbol("attribute"), symbol(key)});
  }
}


void nn_info::dump_method_parameters(const std::string& method) {
  if (!has_model) {
    cerr << "please set a model before" << endl;
    return;
  }
  auto method_props = m_model_info.method_properties;
  if (method_props.find(method) == method_props.end()) {
    cerr << "method " << method << " does not seem to be valid." << endl;
    return;
  }
  // Iterating over the keys
  auto outlet = m_outlets[3].get();
  auto params = m_model_info.method_properties[method];  
  outlet->send({symbol(params.name), symbol("channels_in"), params.channels_in});
  outlet->send({symbol(params.name), symbol("channels_out"), params.channels_in});
  outlet->send({symbol(params.name), symbol("ratio_in"), params.ratio_out});
  outlet->send({symbol(params.name), symbol("ratio_out"), params.ratio_out});
}

void nn_info::dump_attribute_parameters(const std::string& attribute) {
  if (!has_model) {
    cerr << "please set a model before" << endl;
    return;
  }
  auto attribute_props = m_model_info.attribute_properties;
  if (attribute_props.find(attribute) == attribute_props.end()) {
    cerr << "attribute " << attribute << " does not seem to be valid." << endl;
    return;
  }
  // Iterating over the keys
  auto outlet = m_outlets[3].get();
  auto params = m_model_info.attribute_properties[attribute];  
  atoms attr_types = {symbol(params.name), symbol("attribute_type")};
  for (auto attr_type: params.attribute_types) 
    attr_types.push_back(symbol(attr_type));
  outlet->send(attr_types); 
}

void nn_info::dump_dictionary() {
  // if (!has_model) {
  //   cerr << "please set a model before" << endl; 
  // }
  // auto dict = nn_tools::dict_from_model_info(m_model_info);
  auto outlet = m_outlets[4].get();
  auto dict_ref = m_model_dict.get();
  if (dict_ref->valid()) {
    std::string name = dict_ref->name();
    outlet->send({"dictionary", symbol(name)});
  } else {
    cerr << "internal dictionary (id: )" << dict_ref->name() << " is invalid." << endl;
  }
}

void nn_info::dump_downloadable_models() {
  auto outlet = m_outlets[5].get(); 
  if (m_available_models_dict.get() == nullptr) {
    m_available_models_dict = std::make_unique<dict>(symbol(true));
  }   
  try {
    if (m_downloader.get()->is_ready()) {
      m_downloader.get()->fill_dict(m_available_models_dict.get());
      outlet->send({"dictionary", symbol(m_available_models_dict.get()->name())});
    }
  } catch (std::string& e) {
    cerr << "could not get models from api" << endl;
    cerr << "reason : " << e << endl;
  }
}

void nn_info::dump_object() {
  if (!has_model) {
    cerr << "please set a model before" << endl; 
  }
  this->dump_methods(); 
  this->dump_attributes(); 
  for (const auto& pair : m_model_info.attribute_properties)
    this->dump_attribute_parameters({pair.first});
  for (const auto& pair : m_model_info.method_properties)
    this->dump_method_parameters({pair.first});
  this->dump_dictionary(); 
  this->dump_downloadable_models(); 
}

nn_info::~nn_info() {

}

MIN_EXTERNAL(nn_info);
