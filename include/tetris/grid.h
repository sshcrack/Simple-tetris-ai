#pragma once

#include "include/tetris/piece.h"
#include <vector>

namespace tetris {

class grid_t {
public:
  piece_t piece;
  bool gameOver;
  float clearedLines;
  std::vector<std::vector<int>> matrix;
  int score;

  grid_t();
  void update();
  void fix_piece();
  void clear_lines();
  void rotate_piece();
  void gravity(int val);
  void move_piece(int dir, int ind);
};

} // namespace tetris
