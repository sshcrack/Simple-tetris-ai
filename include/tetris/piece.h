#pragma once

#include <cstdint>
#include <random>
#include <vector>

namespace tetris {

class piece_t {
public:
  piece_t();
  void new_shape();
  void new_next();

  [[nodiscard]] const std::vector<int>& get_piece_bag() const noexcept {
    return piece_bag;
  }

  void set_piece_bag(const std::vector<int>& bag) noexcept {
    piece_bag = bag;
  }

  [[nodiscard]] int get_n() const noexcept {
    return n;
  }

  void set_n(int value) noexcept {
    n = value;
  }

  [[nodiscard]] int get_m() const noexcept {
    return m;
  }

  void set_m(int value) noexcept {
    m = value;
  }

  [[nodiscard]] int get_next() const noexcept {
    return next;
  }

  void set_next(int value) noexcept {
    next = value;
  }

  [[nodiscard]] uint16_t get_shape() const noexcept {
    return shape;
  }

  void set_shape(uint16_t value) noexcept {
    shape = value;
  }

  [[nodiscard]] const int* get_position() const noexcept {
    return position;
  }

  void set_position(int row, int col) noexcept {
    position[0] = row;
    position[1] = col;
  }

  void set_position_row(int row) noexcept {
    position[0] = row;
  }

  void set_position_col(int col) noexcept {
    position[1] = col;
  }

  [[nodiscard]] bool is_fixed() const noexcept {
    return fixed;
  }

  void set_fixed(bool value) noexcept {
    fixed = value;
  }

private:
  std::vector<int> piece_bag;
  int n, m, next;
  uint16_t shape;
  int position[2];
  bool fixed;
};

} // namespace tetris
