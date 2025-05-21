#include "parsing_utils.h"

bool to_bool(std::string str) {
  if ((str == "0") || (str == "false")) {
    return false;
  } else {
    return true;
  }
}

int to_int(std::string str) { return stoi(str); }

float to_float(std::string str) { return stof(str); }