#pragma once

#include "include/tetris/piece.h"
#include <vector>

namespace tetris {

class grid_t {
public:
  grid_t();
  void update();
  void fix_piece();
  void clear_lines();
  void rotate_piece();
  void gravity(int val);
  void move_piece(int dir, int ind);

  [[nodiscard]] piece_t& get_piece() noexcept {
    return piece;
  }

  [[nodiscard]] const piece_t& get_piece() const noexcept {
    return piece;
  }

  [[nodiscard]] bool is_game_over() const noexcept {
    return gameOver;
  }

  void set_game_over(bool value) noexcept {
    gameOver = value;
  }

  [[nodiscard]] float get_cleared_lines() const noexcept {
    return clearedLines;
  }

  void set_cleared_lines(float value) noexcept {
    clearedLines = value;
  }

  [[nodiscard]] const std::vector<std::vector<int>>& get_matrix() const noexcept {
    return matrix;
  }

  [[nodiscard]] std::vector<std::vector<int>>& get_matrix() noexcept {
    return matrix;
  }

  [[nodiscard]] int get_score() const noexcept {
    return score;
  }

  void set_score(int value) noexcept {
    score = value;
  }

private:
  piece_t piece;
  bool gameOver;
  float clearedLines;
  std::vector<std::vector<int>> matrix;
  int score;
};

} // namespace tetris
