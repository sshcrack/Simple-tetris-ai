#pragma once

#include <random>
#include <string>
#include <vector>

namespace tetris {

class piece_t {
public:
  piece_t();
  void new_shape();
  void new_next();

  [[nodiscard]] const std::vector<int>& get_piece_bag() const noexcept;
  void set_piece_bag(const std::vector<int>& bag) noexcept;

  [[nodiscard]] int get_n() const noexcept;
  void set_n(int value) noexcept;

  [[nodiscard]] int get_m() const noexcept;
  void set_m(int value) noexcept;

  [[nodiscard]] int get_next() const noexcept;
  void set_next(int value) noexcept;

  [[nodiscard]] const std::string& get_shape() const noexcept;
  void set_shape(const std::string& value) noexcept;

  [[nodiscard]] const int* get_position() const noexcept;
  void set_position(int row, int col) noexcept;
  void set_position_row(int row) noexcept;
  void set_position_col(int col) noexcept;

  [[nodiscard]] bool is_fixed() const noexcept;
  void set_fixed(bool value) noexcept;

  [[nodiscard]] const std::vector<std::vector<std::string>>& get_shape_list() const noexcept;

  [[nodiscard]] const std::vector<int>& get_shape_rotations() const noexcept;

private:
  std::vector<int> pieceBag;
  int n, m, next;
  std::string shape;
  int position[2];
  bool fixed;
  std::vector<std::vector<std::string>> shapeList;
  std::vector<int> shapeRotations;
};

} // namespace tetris
