
#include <chrono>
#include <semaphore>
#include <string>
#include <thread>
#include <vector>
#include <algorithm>
#include <atomic>
#include "c74_min.h"
#include "max_model_download.h"

#ifndef CLASS_FLAG_OBJECTAWARE
#define CLASS_FLAG_OBJECTAWARE (0x10000000L) ///< this object can work with 'array' and 'string' objects
#endif

#include "buffer_tools.h"
#include "dict_utils.h"
#include "../../../backend/backend.h"
#include "../../../shared/circular_buffer.h"
#include "../../../shared/static_buffer.h"

#ifndef VERSION
#define VERSION "UNDEFINED"
#endif

#ifndef REFRESH_THREAD_INTERVAL
#define REFRESH_THREAD_INTERVAL 200
#endif

#ifndef DEFAULT_BUFFER_SIZE
#define DEFAULT_BUFFER_SIZE 4096
#endif

using namespace c74::min;

#define DEBUG 0
#ifdef DEBUG
    #if DEBUG == 1
        #define DEBUG_PRINT(fmt, ...) printf("DEBUG: " fmt "\n", ##__VA_ARGS__)
    #else
        #define DEBUG_PRINT(fmt, ...)
    #endif
#else
    #define DEBUG_PRINT(fmt, ...)
#endif


unsigned power_ceil(unsigned x) {
  if (x <= 1)
    return 1;
  int power = 2;
  x--;
  while (x >>= 1)
    power <<= 1;
  return power;
}

// ---------------------------------------------------------------------------
// DspState
// Everything the audio thread and the compute thread share for one model
// configuration: buffer sizes, method, channel layout and the buffers
// themselves. A DspState is built entirely on a non-audio thread, then handed
// over to the audio thread (see nn_base::swap_pending_state), which only swaps
// it in when the compute thread is guaranteed to be idle. Once active, a state
// is never resized or reallocated, so sizes, pointers and params always agree.
// ---------------------------------------------------------------------------
struct DspState {
  bool valid = false;           // false -> output silence
  std::string method;
  int buffer_size = 0;          // n_vec given to the backend
  int model_in = 0;             // model input channels (per batch)
  int model_out = 0;            // model output channels (per batch)
  int n_batches = 1;            // batches given to the backend
  int n_out_channels = 0;       // out channels per batch given to the backend

  // frontend-specific layout (mc.nn~ / mcs.nn~)
  std::vector<long> channel_map;
  int batches_out = 1;
  int out_channels = 0;

  // circular buffers (audio thread <-> model buffers)
  int buffer_in = 0, buffer_out = 0;
  std::unique_ptr<circular_buffer<double, float>[]> in_buffer;
  std::unique_ptr<circular_buffer<float, double>[]> out_buffer;

  // model buffers (audio thread <-> compute thread)
  std::vector<std::unique_ptr<float[]>> in_model, out_model;
  std::vector<float *> in_ptrs, out_ptrs;

  void allocate(int n_buffer_in, int n_buffer_out, int n_model_in, int n_model_out) {
    buffer_in = std::max(0, n_buffer_in);
    buffer_out = std::max(0, n_buffer_out);
    in_buffer = std::make_unique<circular_buffer<double, float>[]>(buffer_in);
    for (int i = 0; i < buffer_in; i++)
      in_buffer[i].initialize(buffer_size);
    out_buffer = std::make_unique<circular_buffer<float, double>[]>(buffer_out);
    for (int i = 0; i < buffer_out; i++)
      out_buffer[i].initialize(buffer_size);

    in_model.clear(); in_ptrs.clear();
    for (int i = 0; i < n_model_in; i++) {
      in_model.push_back(std::make_unique<float[]>(buffer_size)); // zero-initialised
      in_ptrs.push_back(in_model.back().get());
    }
    out_model.clear(); out_ptrs.clear();
    for (int i = 0; i < n_model_out; i++) {
      out_model.push_back(std::make_unique<float[]>(buffer_size));
      out_ptrs.push_back(out_model.back().get());
    }
  }
};

template <typename nn_name, typename op_type = vector_operator<>> 
class nn_base : public object<nn_name>, public op_type {
public:
  
  using BufferList = std::map<std::string, StaticBuffer<float>>;

  nn_base(const atoms &args = {});
  ~nn_base<nn_name, op_type>(); 
  static std::string get_external_name() { return ""; } 

  // INLETS OUTLETS
  std::vector<std::unique_ptr<inlet<>>> m_inlets;
  std::vector<std::unique_ptr<outlet<>>> m_outlets;
  
  virtual void init_model(); 
  virtual void init_downloader(); 
  virtual void init_inputs_and_outputs(const atoms& args); 
  virtual void init_inlets_and_outlets();
  virtual void init_process();
  virtual int get_sample_rate() = 0;
  virtual void init_external(const atoms &args) {
    init_model(); 
    init_downloader(); 
    if (!args.size()) { return; } 
    init_inputs_and_outputs(args);
    init_inlets_and_outlets();
    init_process();
    request_rebuild();
  } 

  // BACKEND RELATED MEMBERS
  std::unique_ptr<Backend> m_model;
  bool m_is_backend_init = false;
  bool m_ready = false;
  std::string m_method;
  std::vector<std::string> settable_attributes;
  c74::min::path m_path;
  // model configuration, as seen by the message thread. The audio / compute
  // threads never read these: they use the active DspState instead.
  int n_inlets, m_model_in, m_in_ratio, n_outlets, m_model_out, m_out_ratio, m_higher_ratio, n_batches;
  bool has_settable_attribute(std::string attribute);
  bool is_valid_print_key(std::string string);
  void update_model(const std::string &model);
  virtual void load_model(const std::string &model);
  virtual void set_method(std::string method);  
  virtual void update_method(std::string method = ""); 
  path to_model_path(std::string model_path);
  
  // BUFFER ATTRIBUTES MANAGER
  BufferManager m_buffer_manager;

  // BUFFER SIZE AS REQUESTED BY THE USER (-1: unset, 0: no thread)
  int m_buffer_size;
  int effective_buffer_size();

  // DSP STATE HANDOVER
  //  - request_rebuild() (non-audio threads) builds a new DspState and posts
  //    it to m_pending_state.
  //  - swap_pending_state() (audio thread) installs it as m_active_state only
  //    when the compute thread is idle; the previous state goes to
  //    m_retired_state and is freed by the next request_rebuild().
  std::atomic<DspState *> m_active_state{nullptr};
  std::atomic<DspState *> m_pending_state{nullptr};
  std::atomic<DspState *> m_retired_state{nullptr};
  virtual std::unique_ptr<DspState> build_state();
  void request_rebuild();
  DspState *swap_pending_state();

  // AUDIO PERFORM
  bool m_force_refresh, m_use_thread;
  std::atomic<bool> m_should_stop_perform_thread;
  std::binary_semaphore m_data_available_lock, m_result_available_lock;
  std::unique_ptr<std::thread> m_compute_thread;
  void run_model(DspState &state);
  void compute_loop();

  void operator()(audio_bundle input, audio_bundle output);
  virtual void perform(DspState &state, audio_bundle input, audio_bundle output) { }

  // FLUX
  logger cout;
  logger cerr;
  logger cwarn;

  // HELPERS
  virtual void dump_object();
  void print_to_cout(std::string &message);
  void print_to_cerr(std::string &message);

  // DOWNLOAD RELATED ATTRIBUTES
  std::unique_ptr<MaxModelDownloader> m_downloader;
  void dump_available_models(); 

  // ONLY FOR DOCUMENTATION
  argument<symbol> path_arg{this, "model path",
                            "Absolute path to the pretrained model."};
  argument<symbol> method_arg{this, "method",
                              "Name of the method to call during synthesis."};
  argument<int> buffer_arg{
      this, "buffer size",
      "Size of the internal buffer (can't be lower than the method's ratio)."};

  // ENABLE / DISABLE ATTRIBUTE
  attribute<bool> enable{this, "enable", true,
                         description{"Enable / disable tensor computation"}};

  // ENABLE / DISABLE ATTRIBUTE
  attribute<bool> gpu{this, "gpu", false,
                      description{"Enable / disable gpu usage when available"},
                      setter{[this](const c74::min::atoms &args,
                                    const int inlet) -> c74::min::atoms {
                        if (m_is_backend_init)
                          m_model->use_gpu(bool(args[0]));
                        return args;
                      }}};

  // TRACK BUFFER ATTRIBUTE
  attribute<bool> track_buffers{this, 
                                "track_buffers", 
                                false, 
                                description{"tracks buffer change for buffer attributes"}, 
                                setter{
                                  MIN_FUNCTION{
                                    bool enable_buffer_tracking = args[0];
                                    if (m_is_backend_init) {
                                      this->m_buffer_manager.set_buffer_tracking(enable_buffer_tracking);
                                    }
                                    return args;
                                  }}};


  // BOOT STAMP
  // message<> maxclass_setup;  
  message<> print {
    this, "print", 
    MIN_FUNCTION {
      bool is_valid = is_valid_print_key(args[0]);
      if (!is_valid) {
        return {};
      }
      DEBUG_PRINT("asked to print %s on %s", std::string(args[2]).c_str(), std::string(args[1]).c_str());
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

  message<> buffer_notify{
      this, "notify",
        MIN_FUNCTION {
          return buffer_reference::handle_notification<BufferManager::iterator>(
            this, 
            args, 
            m_buffer_manager.begin(), 
            m_buffer_manager.end()
          );
      }};

  message<> dump_callback{
      this, "dump", 
        description{"dumps model information to console"},
        MIN_FUNCTION {
          this->dump_object();  
          return {};
        }};

  message <> get_available_models_callback{
    this, "print_available_models", 
    description{"print available models to console"}, 
    MIN_FUNCTION {
        this->dump_available_models(); 
        return {};
    }};

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

  message <> update_method_fn {
    this, "method", 
    description{"set the current method of model"}, 
    MIN_FUNCTION {
      if (args.size() == 0) {
        cerr << "please provide a method" << endl; 
      }
      std::string method = args[0]; 
      try {
        if (m_model->is_loaded()) {
          if (!m_model->has_method(method)) {
            cerr << "current model does not have method " << method << endl; 
            return args; 
          }
        }
        this->set_method(method);
        this->update_method();
        this->request_rebuild(); 
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

  message<> anything{
      this, "anything", "callback for attributes",
      [this](const c74::min::atoms &args, const int inlet) -> c74::min::atoms {
        symbol attribute_name = args[0];
        if (attribute_name == "reload") {
          try {
            m_model->reload();
          } catch (std::string &e) {
            cerr << e << endl;
          }
          request_rebuild();
        } else if (attribute_name == "load") {
          if (args.size() < 2) {
            cerr << "a model path must be given along the model message" << endl; 
          } else {
            std::string model_path = args[1];
            update_model(model_path); 
          }
          return {}; 
        } else if (attribute_name == "get_attributes") {
          for (std::string attr : settable_attributes) {
            cout << attr << endl;
          }
          return {};
        } else if (attribute_name == "get_methods") {
          for (std::string method : m_model->get_available_methods())
            cout << method << endl;
          return {};
        } else if (attribute_name == "get") {
          if (args.size() < 2) {
            cerr << "get must be given an attribute name" << endl;
            return {};
          }
          attribute_name = args[1];
          if (m_model->has_settable_attribute(attribute_name)) {
            try {
              cout << attribute_name << ": "
                 << m_model->get_attribute_as_string(attribute_name) << endl;
            } catch (std::string& e) {
              cout << e << endl; 
            }
          } else {
            cerr << "no attribute " << attribute_name << " found in model"
                 << endl;
          }
          return {};
        } else if (attribute_name == "set") {
          if (args.size() < 3) {
            cerr << "set must be given an attribute name and corresponding "
                    "arguments"
                 << endl;
            return {};
          }
          attribute_name = args[1];
          std::vector<std::string> attribute_args;

          BufferList buffers;
          if (has_settable_attribute(attribute_name)) {

            for (int i = 2; i < args.size(); i++) {
              // get if argument is buffer
              attribute_args.push_back(args[i]);
              try {
                m_buffer_manager.append_if_buffer_element(m_model.get(), buffers, args[i], attribute_name, i - 2);
              } catch (std::string &message) {
                cerr << message << endl; 
                return args; 
              }
            }
            try {
              m_model->set_attribute(attribute_name, attribute_args, buffers);
            } catch (std::string message) {
              cerr << message << endl;
            }
          } else {
            cerr << "model does not have attribute " << attribute_name << endl;
          }
        } else {
          cerr << "no corresponding method for " << attribute_name << endl;
        }
        return {};
      }};
};


template <typename nn_name, typename op_type>
bool nn_base<nn_name, op_type>::is_valid_print_key(std::string id_string) {
  if (id_string == m_buffer_manager.string_id()) {
    return true;
  } else if (id_string == m_downloader.get()->string_id()) {
    return true;
  }
  return false;
}

template <typename nn_name, typename op_type>
void nn_base<nn_name, op_type>::init_model() {
  m_model = std::make_unique<Backend>();
  m_is_backend_init = true;
}

template <typename nn_name, typename op_type>
void nn_base<nn_name, op_type>::init_downloader() {
  try {
    m_downloader = std::make_unique<MaxModelDownloader>(this, nn_name::get_external_name()); 
  } catch (std::string &e) {
    cerr << "failed to init downloader. caught exception : " << e << endl;
  } catch (std::exception &e) {
    cerr << "failed to init downloader. caught exception : " << e.what() << endl;
  }
}

template <typename nn_name, typename op_type>
void nn_base<nn_name, op_type>::init_inputs_and_outputs(const atoms &args) {
  DEBUG_PRINT("parsing inputs & outputs...");
  bool empty_mode; 
  if (args.size() > 0) { // ONE ARGUMENT IS GIVEN
    auto model_path = std::string(args[0]);
    if (model_path == "void") {
      empty_mode = true;    
    } else {
      try {
        m_path = to_model_path(model_path);
      } catch (std::string &e) {
        error(e);
      }
      empty_mode = false;
    }
  }
  DEBUG_PRINT("empty mode set to %d", empty_mode);
  
  if (empty_mode) {
    if (args.size() > 1) { // FOUR ARGUMENTS ARE GIVEN
      auto inlets_arg = int(args[1]);
      if (inlets_arg >= 1) { n_inlets = inlets_arg; }
    }
    if (args.size() > 2) { // FIVE ARGUMENTS ARE GIVEN
      auto outlets_arg = int(args[2]);
      if (outlets_arg >= 1) { n_outlets = outlets_arg; }
    } 
    if (args.size() > 3) { // THREE ARGUMENTS ARE GIVEN
      m_buffer_size = int(args[3]);
    }
    if (n_outlets == -1) { n_outlets = 1;}
    DEBUG_PRINT("empty mode");
    DEBUG_PRINT("%d inlets", n_inlets);
    DEBUG_PRINT("%d outlets", n_outlets);
  } else {
    if (args.size() > 1) { // TWO ARGUMENTS ARE GIVEN
      m_method = std::string(args[1]);
    }
    if (args.size() > 2) { // THREE ARGUMENTS ARE GIVEN
      m_buffer_size = int(args[2]);
    }
    if (args.size() > 3) { // FOUR ARGUMENTS ARE GIVEN
      auto inlets_arg = int(args[3]);
      if (inlets_arg >= 1) { n_inlets = inlets_arg; }
    }
    if (args.size() > 4) { // FIVE ARGUMENTS ARE GIVEN
      auto outlets_arg = int(args[4]);
      if (outlets_arg >= 1) { n_outlets = outlets_arg; }
    }
    DEBUG_PRINT("loading model : %s", std::string(m_path).c_str()); 
    DEBUG_PRINT("%d inlets", n_inlets);
    DEBUG_PRINT("%d outlets", n_outlets);
    load_model(m_path);
    if ((!m_method.empty()) && (m_model->is_loaded()))
      update_method(); 
    if ((n_inlets == -1) || (n_outlets == -1)) {
      error("could not initialise object");
    }
  }
}

template <typename nn_name, typename op_type>
void nn_base<nn_name, op_type>::init_inlets_and_outlets() {
  for (int i(0); i < n_inlets; i++) {
    std::string input_label = "";
    try {
      input_label = m_model->get_model()
                        .attr(m_method + "_input_labels")
                        .toList()
                        .get(i)
                        .toStringRef();
    } catch (...) {
      input_label = "(signal) model input " + std::to_string(i);
    }
    m_inlets.push_back(std::make_unique<inlet<>>(this, input_label, "signal"));
  }

  for (int i(0); i < n_outlets; i++) {
    std::string output_label = "";
    try {
      output_label = m_model->get_model()
                         .attr(m_method + "_output_labels")
                         .toList()
                         .get(i)
                         .toStringRef();
    } catch (...) {
      output_label = "(signal) model output " + std::to_string(i);
    }
    m_outlets.push_back(
        std::make_unique<outlet<>>(this, output_label, "signal"));
  }

}

template <typename nn_name, typename op_type>
int nn_base<nn_name, op_type>::effective_buffer_size() {
  int higher_ratio = std::max(1, m_higher_ratio);
  if (m_buffer_size <= 0) {
    // -1 (unset) or 0 (no thread mode) : smallest possible buffer
    return higher_ratio;
  }
  if (m_buffer_size < higher_ratio) {
    cerr << "buffer size too small, switching to " << higher_ratio << endl;
    return higher_ratio;
  }
  return power_ceil(m_buffer_size);
}

template <typename nn_name, typename op_type>
std::unique_ptr<DspState> nn_base<nn_name, op_type>::build_state() {
  auto state = std::make_unique<DspState>();
  if (!m_ready || !m_model || !m_model->is_loaded())
    return state;
  auto params = m_model->get_method_params(m_method);
  if (params.size() < 4)
    return state;
  state->method = m_method;
  state->model_in = params[0];
  state->model_out = params[2];
  state->buffer_size = effective_buffer_size();
  state->n_batches = 1;
  state->n_out_channels = state->model_out;
  state->allocate(n_inlets, n_outlets, state->model_in, state->model_out);
  state->valid = true;
  return state;
}

template <typename nn_name, typename op_type>
void nn_base<nn_name, op_type>::request_rebuild() {
  // must NOT be called from the audio thread (allocates, may lock the model)
  DspState *state = build_state().release();
  delete m_retired_state.exchange(nullptr, std::memory_order_acq_rel);
  // if a previous pending state was never picked up, drop it
  delete m_pending_state.exchange(state, std::memory_order_acq_rel);
}

template <typename nn_name, typename op_type>
DspState *nn_base<nn_name, op_type>::swap_pending_state() {
  // audio thread. No allocation, no lock.
  DspState *active = m_active_state.load(std::memory_order_acquire);
  if (m_pending_state.load(std::memory_order_relaxed) == nullptr)
    return active;

  // In thread mode, m_result_available_lock is only available when the
  // compute thread is idle (it is held from the moment the audio thread
  // hands data over until the forward pass is done). Holding it guarantees
  // the compute thread is not using the active state while we swap.
  if (m_use_thread && !m_result_available_lock.try_acquire())
    return active; // compute thread busy, retry on next vector

  DspState *next = m_pending_state.exchange(nullptr, std::memory_order_acq_rel);
  if (next) {
    m_active_state.store(next, std::memory_order_release);
    // hand the old state back to the message thread for deletion.
    // (the slot is always emptied by request_rebuild() before a new state is
    // posted, so this delete is a never-expected fallback)
    delete m_retired_state.exchange(active, std::memory_order_acq_rel);
    active = next;
  }

  if (m_use_thread)
    m_result_available_lock.release();
  return active;
}

template <typename nn_name, typename op_type>
void nn_base<nn_name, op_type>::run_model(DspState &state) {
  m_model->perform(state.in_ptrs, state.out_ptrs, state.method,
                   state.n_batches, state.n_out_channels, state.buffer_size);
}

template <typename nn_name, typename op_type>
void nn_base<nn_name, op_type>::compute_loop() {
  while (!m_should_stop_perform_thread.load()) {
    if (m_data_available_lock.try_acquire_for(
            std::chrono::milliseconds(REFRESH_THREAD_INTERVAL))) {
      // the audio thread only releases m_data_available_lock after filling
      // the active state's buffers, and cannot swap the state before we
      // release m_result_available_lock.
      DspState *state = m_active_state.load(std::memory_order_acquire);
      if (state && state->valid)
        run_model(*state);
      m_result_available_lock.release();
    }
  }
}

template <typename nn_name, typename op_type>
void nn_base<nn_name, op_type>::init_process() {
  if (m_buffer_size == 0) {
    m_use_thread = false;
  }
  // A single long-lived compute thread per object: libtorch leaks thread-local
  // state for every thread that runs a model (notably on Windows, see
  // https://github.com/pytorch/pytorch/issues/24237), so never spawn one per buffer.
  if (m_use_thread && !m_compute_thread) {
    m_compute_thread = std::make_unique<std::thread>(&nn_base<nn_name, op_type>::compute_loop, this);
  }
}

template <typename nn_name, typename op_type>
nn_base<nn_name, op_type>::nn_base(const atoms &args)
    : m_compute_thread(nullptr), n_inlets(-1), m_model_in(0), m_in_ratio(1), n_outlets(-1), m_model_out(0),
      m_out_ratio(1), m_higher_ratio(1), m_buffer_size(-1), n_batches(1),  
      m_method("forward"), m_force_refresh(false),
      m_use_thread(true), m_data_available_lock(0), m_result_available_lock(1),
      m_should_stop_perform_thread(false),
      cout(this, logger::type::message), cwarn(this, logger::type::warning), cerr(this, logger::type::error) 
{
  init_external(args);
}



template <typename nn_name, typename op_type>
nn_base<nn_name, op_type>::~nn_base() {
  m_should_stop_perform_thread = true;
  if (m_compute_thread)
    m_compute_thread->join();
  delete m_pending_state.exchange(nullptr);
  delete m_active_state.exchange(nullptr);
  delete m_retired_state.exchange(nullptr);
}

template <typename nn_name, typename op_type>
path nn_base<nn_name, op_type>::to_model_path(std::string model_path) {
  try {
    if (model_path.substr(model_path.length() - 3) != ".ts")
      model_path = model_path + ".ts";
    // first look in "models" subfolder
    if (m_downloader) {
      auto download_path = m_downloader->get_download_path() / model_path; 
      if (std::filesystem::exists(download_path)) {
        return path(download_path.string());
      }
    }
    auto parsed_path = path(model_path);
    return parsed_path;
  } catch (std::string &e) {
    throw e; 
  } catch (std::exception& e) {
    std::string exception_str = e.what();
    throw exception_str;
  }
}

template <typename nn_name, typename op_type>
void nn_base<nn_name, op_type>::update_model(const std::string &model) {
  load_model(model);
  if ((m_model->is_loaded()) && m_model->has_method(m_method)) {
    update_method();
  }
  request_rebuild();
}

template <typename nn_name, typename op_type>
void nn_base<nn_name, op_type>::set_method(std::string method) {
  m_method = method;
}

template <typename nn_name, typename op_type>
void nn_base<nn_name, op_type>::update_method(std::string method) {
  try {
    // if (!m_model->is_loaded()) {
    //   cerr << "no model is set yet" << endl;
    //   return; 
    // }
    if (method.empty()) {
      if (m_method.empty()) {
        return;
      } 
      method = m_method; 
    }
    if (m_model->has_method(method)) {
      auto params = m_model->get_method_params(method);
      if (params.size() == 0) {
        throw std::string("method ") + method + " present in model, but not initialised";
      }
      // input parameters
      m_model_in = params[0];
      if (n_inlets == -1) {
        n_inlets = m_model_in;
      } 
      m_in_ratio = params[1];

      // output parameters
      m_model_out = params[2];
      if (n_outlets == -1) {
        n_outlets = params[2];
      }
      m_out_ratio = params[3]; 

      if (m_model_in != n_inlets) {
        cwarn << "nn_base~ has been initialised with " << n_inlets << " inputs, but current model has " << m_model_in << endl;
      }
      if (m_model_out != n_outlets) {
        cwarn << "nn_base~ has been initialised with " << n_outlets << " outputs, but current model has " << m_model_out << endl;
      }
      
      set_method(method);
      m_ready = true;
    } else {
      cerr << "model " << m_path << " does not have method " << method << endl; 
      m_ready = false;
    }
  } catch (std::string &e) {
    cerr << "failed to set method to " << method << ". Caught exception : " << e << endl; 
  } catch (std::exception &e) {
    cerr << "failed to set method to " << method << ". Caught exception : " << e.what() << endl; 
  }

}

template <typename nn_name, typename op_type>
void nn_base<nn_name, op_type>::load_model(const std::string& model_path) {
  DEBUG_PRINT("model path: %s", model_path.c_str());
  if (m_method.empty()) 
    m_method = "forward";

  try {
    auto path = to_model_path(model_path);
    m_model.get()->load(path, get_sample_rate()); 
    DEBUG_PRINT("model loaded"); 
    m_model->use_gpu(gpu);
    m_higher_ratio = m_model->get_higher_ratio();
    settable_attributes = m_model->get_settable_attributes(); 
    DEBUG_PRINT("attributes setted"); 
    m_buffer_manager.init_buffer_list(m_model.get(), this);
    m_ready = true; 

    if (!m_model->has_method(m_method)) {
      cwarn << model_path << " loaded, but does not have method " << m_method << endl; 

    }
  } catch (std::string &e) {
    cerr << e << endl;
    return;
  }
}

template <typename nn_name, typename op_type>
bool nn_base<nn_name, op_type>::has_settable_attribute(std::string attribute) {
  for (std::string candidate: settable_attributes) {
    if (candidate == attribute)
      return true;
  }
  return false;
}

template <typename nn_name, typename op_type>
void nn_base<nn_name, op_type>::operator()(audio_bundle input, audio_bundle output) {
  // install a newly built state if one is pending (safe point)
  DspState *state = swap_pending_state();

  // CHECK IF MODEL IS LOADED AND ENABLED
  if (!enable || state == nullptr || !state->valid) {
    fill_with_zero(output);
    return;
  }

  // CHECK IF DSP_VEC_SIZE IS LARGER THAN BUFFER SIZE
  auto dsp_vec_size = output.frame_count();
  if (dsp_vec_size > state->buffer_size) {
    cerr << "vector size (" << dsp_vec_size << ") ";
    cerr << "larger than buffer size (" << state->buffer_size << "). ";
    cerr << "disabling model.";
    cerr << endl;
    enable = false;
    fill_with_zero(output);
    return;
  }
  perform(*state, input, output);
}

template <typename nn_name, typename op_type>
void nn_base<nn_name, op_type>::dump_object() {
  cout << "model_path: " << std::string(m_path) << endl;
  cout << "input dimension: " << std::to_string(m_inlets.size()) << endl;
  cout << "output dimension: " << std::to_string(m_outlets.size()) << endl;
  cout << "input ratio: " << std::to_string(m_in_ratio) << endl; 
  cout << "output ratio: " << std::to_string(m_out_ratio) << endl; 
  cout << "methods: ";
  for (auto method: m_model->get_available_methods())
    cout << method << "; ";
  cout << endl; 
  cout << "attributes: ";
  for (auto attribute: m_model->get_settable_attributes())
    cout << attribute << "; ";
  cout << endl;
}

template <typename nn_name, typename op_type>
void nn_base<nn_name, op_type>::print_to_cout(std::string &message) {
  cout << message << endl;
}

template <typename nn_name, typename op_type>
void nn_base<nn_name, op_type>::print_to_cerr(std::string &message) {
  cerr << message << endl;
}

template <typename nn_name, typename op_type>
void nn_base<nn_name, op_type>::dump_available_models() {
  try {
    if (m_downloader.get()->is_ready()) {
      m_downloader.get()->print_available_models();
    }
  } catch (std::string &e) {
    cerr << "error from model downloader : " << e << endl;
  }
}
