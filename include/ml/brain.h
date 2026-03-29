#pragma once

#include "include/ml/neural-net.h"
#include "include/ml/utils.h"

#include "include/tetris/grid.h"

#include <vector>

namespace ml {

class brain_t {
public:
  brain_t();
  brain_t(neural_net_t net);

  brain_t crossover(brain_t partner_param) noexcept;
  std::string best_move(const tetris::grid_t& grid) noexcept;

  void set_score(int score) noexcept {
    this->score = score;
  }

  void save(const std::string& fileName) {
    ml::save(params, fileName, score);
  }

  [[nodiscard]] int get_score() const noexcept {
    return score;
  }

private:
  std::vector<int> get_column_heights(const std::vector<std::vector<int>>& grid) noexcept;
  int get_holes(const std::vector<std::vector<int>>& grid) noexcept;
  int get_bumpiness(const std::vector<std::vector<int>>& grid) noexcept;
  int get_completed_lines(const std::vector<std::vector<int>>& grid) noexcept;
  int get_aggregate_height(const std::vector<std::vector<int>>& grid) noexcept;
  int get_wells(const std::vector<std::vector<int>>& grid) noexcept;

  int score;
  neural_net_t params;
};

} // namespace ml
