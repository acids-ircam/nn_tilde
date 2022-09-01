#include "parsing_utils.h"

bool to_bool(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    std::istringstream is(str);
    bool b;
    is >> std::boolalpha >> b;
    return b;
}

int to_int(std::string str) {
    return stoi(str);
}

float to_float(std::string str) {
    return stof(str);
}