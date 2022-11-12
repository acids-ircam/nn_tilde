#pragma once
#include <string>

// Abstract class for logging, that has to be overriden for specific environments
class Logger {
public: 
    Logger() {
    }

    void post(std::string message)
    {
    }

    void error(std::string message)
    {
    }
};