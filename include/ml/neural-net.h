#pragma once

#include <vector>

namespace ml {

// Number of inputs to the neural network:
// 0: aggregate_height, 1: completed_lines, 2: holes, 3: bumpiness, 4: wells
inline constexpr int NET_INPUTS = 5;
inline constexpr int NET_HIDDEN = 3;

struct neural_net_t {
  float bias2;
  std::vector<float> layer2;
  std::vector<float> biases1;
  std::vector<std::vector<float>> layer1;
};

} // namespace ml
