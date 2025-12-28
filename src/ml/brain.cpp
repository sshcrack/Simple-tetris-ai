#include "include/ml/brain.h"

#include "include/ml/utils.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <string>

namespace ml {

brain_t::brain_t() {
  params = generate_net();
  score = 0;
}

brain_t::brain_t(neural_net_t params) {
  this->params = params;
  score = 0;
}

std::string brain_t::best_move(tetris::grid_t grid) noexcept {
  std::string bestMove, bestRotation;
  float maxScore = -pow(10.0, 5);
  int nRotations = grid.piece.shapeRotations[grid.piece.n];
  tetris::grid_t startinggrid_t = grid;
  std::vector<std::string> possibleMoves = {"llll", "lll", "ll", "l", "rrrrr", "rrrr", "rrr", "rr", "r", ""};

  for (int r = 0; r < nRotations; r++) {
    for (int m = 0; m < 10; m++) {

      grid = startinggrid_t;

      for (int i = 0; i < r; i++)
        grid.rotate_piece();

      if (possibleMoves[m][0] == 'l') {
        for (int i = 0; i < possibleMoves[m].size(); i++)
          grid.move_piece(-1, 0);
      } else if (possibleMoves[m][0] == 'r') {
        for (int i = 0; i < possibleMoves[m].size(); i++)
          grid.move_piece(1, 9);
      }

      grid.gravity(3);
      std::vector<int> heuristics = {getAggregateHeight(grid.matrix),
                                     getCompletedLines(grid.matrix),
                                     getHoles(grid.matrix),
                                     getBumpiness(grid.matrix)};

      float score = forward(params, heuristics);

      grid.piece.new_shape();

      int nRotationsNext = grid.piece.shapeRotations[grid.piece.n];
      tetris::grid_t nextStartinggrid_t = grid;

      for (int rNext = 0; rNext < nRotationsNext; rNext++) {
        for (int mNext = 0; mNext < 10; mNext++) {

          grid = nextStartinggrid_t;
          for (int i = 0; i < rNext; i++)
            grid.rotate_piece();

          if (possibleMoves[mNext][0] == 'l') {
            for (int i = 0; i < possibleMoves[mNext].size(); i++)
              grid.move_piece(-1, 0);
          } else if (possibleMoves[mNext][0] == 'r') {
            for (int i = 0; i < possibleMoves[mNext].size(); i++)
              grid.move_piece(1, 9);
          }

          std::vector<int> heuristics = {getAggregateHeight(grid.matrix),
                                         getCompletedLines(grid.matrix),
                                         getHoles(grid.matrix),
                                         getBumpiness(grid.matrix)};

          float score2 = score + forward(params, heuristics);

          if (score2 >= maxScore) {
            maxScore = score2;
            bestMove = possibleMoves[m];
            bestRotation = std::to_string(r);
          }
        }
      }
    }
  }
  return bestMove + bestRotation;
}

brain_t brain_t::crossover(brain_t partner) noexcept {
  brain_t offspring;
  neural_net_t p1 = this->params;
  neural_net_t p2 = partner.params;
  float f1 = this->score;
  float f2 = partner.score;
  int i, j;

  for (i = 0; i < 3; i++) {
    for (j = 0; j < 4; j++) {
      offspring.params.layer1[i][j] = (f1 * p1.layer1[i][j] + f2 * p2.layer1[i][j]) / (f1 + f2 + pow(10, -10));
    }
    offspring.params.biases1[i] = (f1 * p1.biases1[i] + f2 * p2.biases1[i]) / (f1 + f2 + pow(10, -10));
  }

  for (i = 0; i < 3; i++) {
    offspring.params.layer2[i] = (f1 * p1.layer2[i] + f2 * p2.layer2[i]) / (f1 + f2 + pow(10, -10));
  }
  offspring.params.bias2 = (f1 * p1.bias2 + f2 * p2.bias2) / (f1 + f2 + pow(10, -10));

  return offspring;
}

std::vector<int> brain_t::getColumnHeights(std::vector<std::vector<int>> grid) noexcept {
  std::vector<int> columnHeights;
  for (int col = 0; col < 10; col++) {
    int height = 20;
    for (int row = 4; row < 24; row++) {
      if (grid[row][col] != 0 && grid[row][col] != 1)
        break;
      else
        height--;
    }
    columnHeights.push_back(height);
  }
  return columnHeights;
}

int brain_t::getCompletedLines(std::vector<std::vector<int>> grid) noexcept {
  int completedLines = 0;
  for (int l = 0; l < 24; l++) {
    if (find(grid[l].begin(), grid[l].end(), 0) == grid[l].end()) {
      completedLines++;
    }
  }
  return completedLines;
}

int brain_t::getAggregateHeight(std::vector<std::vector<int>> grid) noexcept {
  int sum = 0;
  std::vector<int> heights = getColumnHeights(grid);
  for (int i = 0; i < 10; i++)
    sum += heights[i];
  return sum;
}

int brain_t::getBumpiness(std::vector<std::vector<int>> grid) noexcept {
  std::vector<int> heights = getColumnHeights(grid);
  int bumpiness = 0;
  for (int i = 0; i < 9; i++)
    bumpiness += abs(heights[i] - heights[i + 1]);
  return bumpiness;
}

int brain_t::getHoles(std::vector<std::vector<int>> grid) noexcept {
  int holes = 0;

  for (int row = 23; row > 3; row--) {
    std::vector<int> zeros = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    if (grid[row] == zeros)
      return holes;

    for (int col = 9; col >= 0; col--) {
      bool covered = false;
      if (grid[row][col] == 0) {
        for (int r = row - 1; r > 3; r--) {
          if (grid[r][col] != 0) {
            covered = true;
            break;
          }
        }
        if (covered)
          holes++;
      }
    }
  }
  return holes;
}

} // namespace ml
