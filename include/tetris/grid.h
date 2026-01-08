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

  [[nodiscard]] piece_t& get_piece() noexcept;
  [[nodiscard]] const piece_t& get_piece() const noexcept;

  [[nodiscard]] bool is_game_over() const noexcept;
  void set_game_over(bool value) noexcept;

  [[nodiscard]] float get_cleared_lines() const noexcept;
  void set_cleared_lines(float value) noexcept;

  [[nodiscard]] const std::vector<std::vector<int>>& get_matrix() const noexcept;
  [[nodiscard]] std::vector<std::vector<int>>& get_matrix() noexcept;

  [[nodiscard]] int get_score() const noexcept;
  void set_score(int value) noexcept;

private:
  piece_t piece;
  bool gameOver;
  float clearedLines;
  std::vector<std::vector<int>> matrix;
  int score;
};

} // namespace tetris
