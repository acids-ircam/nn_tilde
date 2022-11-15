#pragma once
#include "../../../backend/logger.h"
#include "c74_min.h"

template <typename T>  class MaxLogger: public Logger {
    private:
        c74::min::object<T> *object;
        c74::min::logger logger;
        c74::min::logger warning_logger;
        c74::min::logger error_logger;
    public:
        MaxLogger(c74::min::object<T> *object): 
            logger (object, c74::min::logger::type::message),
            warning_logger (object, c74::min::logger::type::warning),  
            error_logger (object, c74::min::logger::type::error) 
        {
            this->object = object;
        }

        void post(std::string message)
        {
            std::cout << message << std::endl;
            this->logger << message << c74::min::endl;
        }

        void warning(std::string message) {
            std::cout << message << std::endl;
            this->warning_logger << message << c74::min::endl;
        }

        void error(std::string message)
        {
            std::cerr << message << std::endl;
            this->error_logger << message << c74::min::endl;
        }
};
