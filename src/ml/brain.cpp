#include "include/ml/brain.h"

#include "include/ml/utils.h"
#include "include/tetris/constants.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <string>

namespace ml {

brain_t::brain_t() {
  params = generate_net();
  score = 0;
}

brain_t::brain_t(neural_net_t net) {
  this->params = net;
  score = 0;
}

std::string brain_t::best_move(const tetris::grid_t& grid) noexcept {
  std::string bestMove;
  std::string bestRotation;
  float max_score = static_cast<float>(-pow(10.0, 5));
  const int n_rotations = tetris::SHAPE_ROTATIONS[grid.get_piece().get_n()];
  tetris::grid_t starting_grid_t = grid;
  std::vector<std::string> possible_moves = {"llll", "lll", "ll", "l", "rrrrr", "rrrr", "rrr", "rr", "r", ""};

  for (int r = 0; r < n_rotations; r++) {
    for (int m = 0; m < 10; m++) {

      tetris::grid_t grid = starting_grid_t;

      for (int i = 0; i < r; i++) {
        grid.rotate_piece();
      }

      if (possible_moves[m][0] == 'l') {
        for (size_t i = 0; i < possible_moves[m].size(); i++) {
          grid.move_piece(-1, 0);
        }
      } else if (possible_moves[m][0] == 'r') {
        for (size_t i = 0; i < possible_moves[m].size(); i++) {
          grid.move_piece(1, 9);
        }
      }

      grid.gravity(3);
      const std::vector<int> heuristics = {get_aggregate_height(grid.get_matrix()),
                                     get_completed_lines(grid.get_matrix()),
                                     get_holes(grid.get_matrix()),
                                     get_bumpiness(grid.get_matrix()),
                                     get_wells(grid.get_matrix())};

      float score = forward(params, heuristics);

      grid.get_piece().new_shape();

      int n_rotations_next = tetris::SHAPE_ROTATIONS[grid.get_piece().get_n()];
      tetris::grid_t next_starting_grid_t = grid;

      for (int rNext = 0; rNext < n_rotations_next; rNext++) {
        for (int mNext = 0; mNext < 10; mNext++) {

          grid = next_starting_grid_t;
          for (int i = 0; i < rNext; i++) {
            grid.rotate_piece();
          }

          if (possible_moves[mNext][0] == 'l') {
            for (size_t i = 0; i < possible_moves[mNext].size(); i++) {
              grid.move_piece(-1, 0);
            }
          } else if (possible_moves[mNext][0] == 'r') {
            for (size_t i = 0; i < possible_moves[mNext].size(); i++) {
              grid.move_piece(1, 9);
            }
          }

          std::vector<int> heuristics = {get_aggregate_height(grid.get_matrix()),
                                         get_completed_lines(grid.get_matrix()),
                                         get_holes(grid.get_matrix()),
                                         get_bumpiness(grid.get_matrix()),
                                         get_wells(grid.get_matrix())};

          const float score2 = score + forward(params, heuristics);

          if (score2 >= max_score) {
            max_score = score2;
            bestMove = possible_moves[m];
            bestRotation = std::to_string(r);
          }
        }
      }
    }
  }
  return bestMove + bestRotation;
}

brain_t brain_t::crossover(brain_t partner_param) noexcept {
  brain_t offspring;
  neural_net_t p1 = this->params;
  neural_net_t p2 = partner_param.params;
  float f1 = this->score;
  float f2 = partner_param.score;

  for (int i = 0; i < NET_HIDDEN; i++) {
    for (int j = 0; j < NET_INPUTS; j++) {
      offspring.params.layer1[i][j] = (f1 * p1.layer1[i][j] + f2 * p2.layer1[i][j]) / (f1 + f2 + pow(10, -10));
    }
    offspring.params.biases1[i] = (f1 * p1.biases1[i] + f2 * p2.biases1[i]) / (f1 + f2 + pow(10, -10));
  }

  for (int i = 0; i < NET_HIDDEN; i++) {
    offspring.params.layer2[i] = (f1 * p1.layer2[i] + f2 * p2.layer2[i]) / (f1 + f2 + pow(10, -10));
  }
  offspring.params.bias2 = (f1 * p1.bias2 + f2 * p2.bias2) / (f1 + f2 + pow(10, -10));

  return offspring;
}

std::vector<int> brain_t::get_column_heights(const std::vector<std::vector<int>>& grid) noexcept {
  std::vector<int> column_heights;
  for (int col = 0; col < 10; col++) {
    int height = 20;
    for (int row = 4; row < 24; row++) {
      if (grid[row][col] != 0 && grid[row][col] != 1) {
        break;
      } else {
        height--;
      }
    }
    column_heights.push_back(height);
  }
  return column_heights;
}

int brain_t::get_completed_lines(const std::vector<std::vector<int>>& grid) noexcept {
  int completed_lines = 0;
  for (int l = 0; l < 24; l++) {
    if (find(grid[l].begin(), grid[l].end(), 0) == grid[l].end()) {
      completed_lines++;
    }
  }
  return completed_lines;
}

int brain_t::get_aggregate_height(const std::vector<std::vector<int>>& grid) noexcept {
  int sum = 0;
  std::vector<int> heights = get_column_heights(grid);
  for (int i = 0; i < 10; i++) {
    sum += heights[i];
  }
  return sum;
}

int brain_t::get_bumpiness(const std::vector<std::vector<int>>& grid) noexcept {
  std::vector<int> heights = get_column_heights(grid);
  int bumpiness = 0;
  for (int i = 0; i < 9; i++) {
    bumpiness += std::abs(heights[i] - heights[i + 1]);
  }
  return bumpiness;
}

int brain_t::get_holes(const std::vector<std::vector<int>>& grid) noexcept {
  int holes = 0;

  for (int row = 23; row > 3; row--) {
    const std::vector<int> zeros = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    if (grid[row] == zeros) {
      return holes;
    }

    for (int col = 9; col >= 0; col--) {
      bool covered = false;
      if (grid[row][col] == 0) {
        for (int r = row - 1; r > 3; r--) {
          if (grid[r][col] != 0) {
            covered = true;
            break;
          }
        }
        if (covered) {
          holes++;
        }
      }
    }
  }
  return holes;
}

int brain_t::get_wells(const std::vector<std::vector<int>>& grid) noexcept {
  std::vector<int> heights = get_column_heights(grid);
  // Only measure edge wells (best for tetris strategy: keep one side column low
  // for I-piece drops). The board edge acts as a wall at height 20.
  const int left_well = std::max(0, heights[1] - heights[0]);
  const int right_well = std::max(0, heights[8] - heights[9]);
  return std::max(left_well, right_well);
}

} // namespace ml
