#include "../shared/base_obj.h"
#include "c74_min.h"

class nn: public nn_base<nn> {

public:
    MIN_DESCRIPTION{"Interface for deep learning models"};
    MIN_TAGS{"audio, deep learning, ai"};
    MIN_AUTHOR{"Antoine Caillon & Axel Chemla--Romeu-Santos"};
};

MIN_EXTERNAL(nn);