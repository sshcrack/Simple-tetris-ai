#pragma once

#include "include/ml/neural-net.h"
#include "include/ml/utils.h"

#include "include/tetris/grid.h"

#include <vector>

namespace ml {

class brain_t {
public:
  brain_t();
  brain_t(neural_net_t params);

  brain_t crossover(brain_t partner) noexcept;
  std::string best_move(tetris::grid_t grid) noexcept;

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
  std::vector<int> getColumnHeights(std::vector<std::vector<int>> grid) noexcept;
  int getHoles(std::vector<std::vector<int>> grid) noexcept;
  int getBumpiness(std::vector<std::vector<int>> grid) noexcept;
  int getCompletedLines(std::vector<std::vector<int>> grid) noexcept;
  int getAggregateHeight(std::vector<std::vector<int>> grid) noexcept;

  int score;
  neural_net_t params;
};

} // namespace ml
