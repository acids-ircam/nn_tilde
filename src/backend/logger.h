#pragma once
#include <string>

// Abstract class for logging, that has to be overriden for specific environments
class Logger {
public: 
    Logger() {
    }

    virtual void post(std::string message)
    {
    }

    virtual void warning(std::string message)
    {
    }

    virtual void error(std::string message)
    {
    }
};

class StdLogger: public Logger {
    public:
        StdLogger() {
        }

        void post(std::string message)
        {
            std::cout << message << std::endl;
        }

        void warning(std::string message)
        {
            std::cout << message << std::endl;
        }

        void error(std::string message)
        {
            std::cerr << message << std::endl;;
        }
};
