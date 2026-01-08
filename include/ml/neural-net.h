#pragma once

#include <vector>

namespace ml {

struct neural_net_t {
  float bias2;
  std::vector<float> layer2;
  std::vector<float> biases1;
  std::vector<std::vector<float>> layer1;
};

} // namespace ml
