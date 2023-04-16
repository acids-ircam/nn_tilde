#include "c74_min.h"
#include "c74_min_unittest.h"
#include "nn_tilde.cpp"
#include <iostream>
#include <math.h>

#define PI 3.14159265

void avgPower(std::vector<float> xs) {
    float power = 0;

    for (auto x : xs) {
        power += x * x;
    }

    power = power / xs.size();
    std::cout << "Avg power: " << power << std::endl;
}


SCENARIO("object produces correct output") {
  ext_main(nullptr);

  
  GIVEN("An instance of nn~ without parameters") {
    nn my_object;
    WHEN("a buffer is given") {
      sample_vector input(4096);
      sample_vector output;
      std::cout << "here" << std::endl;
      for (int i(0); i < 10; i++) {
        for (auto x : input) {
          auto y = my_object(x);
          output.push_back(y);
        }
      }
    }
  }
 
  GIVEN("An instance of nn~ with parameters") {
    atom path("C:/Users/nick/Documents/rave_chugin/chugins/rave/nn_tilde/src/tests/rave_chafe_data_rt.ts"), method("forward");
    atoms args = {path, method};
    nn my_object = nn(args);

    WHEN("a buffer is given") {
      sample_vector input(4096);
      sample_vector output;

      for (int i(0); i < input.size(); i++) {
          input[i] = sin(2 * PI * 220 * i / 44100);
      }

      std::vector<float> result;

      for (int i(0); i < 90; i++) {
          for (int j(0); j < input.size(); j++) {
              auto x = input[j];
              auto y = my_object(x);
              result.push_back(y);
              // std::cout << y << std::endl;
              output.push_back(y);
          }
      }
      avgPower(result);
    }
  }
}