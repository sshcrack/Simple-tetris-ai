#pragma once

#include "include/ml/brain.h"

#include <cstddef>
#include <vector>

namespace ml {

class population_t {
public:
  population_t();

  void generate_new();
  std::vector<float> normalize(std::vector<float> params_param);

  size_t agents_count() const noexcept {
    return agents_.size();
  }

  std::vector<brain_t>& get_agents() noexcept {
    return agents_;
  }

private:
  std::vector<brain_t> agents_;
};

} // namespace ml
