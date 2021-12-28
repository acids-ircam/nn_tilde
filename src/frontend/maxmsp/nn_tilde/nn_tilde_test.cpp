#include "c74_min.h"
#include "c74_min_unittest.h"
#include "nn_tilde.cpp"
#include <iostream>

SCENARIO("object produces correct output") {
  ext_main(nullptr);

  GIVEN("An instance of nn~ without parameters") {
    nn my_object;
    WHEN("a buffer is given") {
      sample_vector input(4096);
      sample_vector output;

      for (int i(0); i < 10; i++) {
        for (auto x : input) {
          auto y = my_object(x);
          output.push_back(y);
        }
      }
    }
  }

  GIVEN("An instance of nn~ with parameters") {
    atom path("/Users/acaillon/Desktop/nn.ts"), method("forward");
    atoms args = {path, method};
    nn my_object = nn(args);

    WHEN("a buffer is given") {
      sample_vector input(4096);
      sample_vector output;

      for (int i(0); i < 10; i++) {
        for (auto x : input) {
          auto y = my_object(x);
          output.push_back(y);
        }
      }
    }
  }
}