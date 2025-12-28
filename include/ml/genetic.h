#pragma once

#include "include/ml/population.h"

namespace ml {

class genetic_t {
public:
  genetic_t();
  void fit();
  brain_t run_games(brain_t brain);

private:
  int gen_;
  int best_recorded_fitnes_;
  population_t population_;
};

} // namespace ml
