#pragma once

#include "include/ml/neural-net.h"

#include <string>
#include <vector>

namespace ml {

float relu(float node) noexcept;

float forward(const neural_net_t& net, const std::vector<int>& inputs) noexcept;

void save(const neural_net_t& net, const std::string& filename, int score) noexcept;

neural_net_t load(const std::string& filename) noexcept;

neural_net_t generate_net() noexcept;

neural_net_t mutate(const neural_net_t& net, float mutation_rate, float mutation_sigma) noexcept;

} // namespace ml
