#pragma once
#include "../../../backend/logger.h"
#include "c74_min.h"

template <typename T>  class MaxLogger: public Logger {
    private:
        c74::min::object<T> *object;
    public:
        MaxLogger(c74::min::object<T> *object) {
            this->object = object;
        }

        void post(std::string message)
        {
            this->object->cout << message << c74::min::endl;
        }

        void error(std::string message)
        {
            c74::min::error(message);
        }
};
