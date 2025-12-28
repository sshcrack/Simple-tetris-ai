#pragma once

#include <random>
#include <string>
#include <vector>

namespace tetris {

class piece_t {
public:
  std::vector<int> pieceBag;
  int n, m, next;
  std::string shape;
  int position[2];
  bool fixed;
  std::vector<std::vector<std::string>> shapeList;
  std::vector<int> shapeRotations;

  piece_t();
  void new_shape();
  void new_next();
};

} // namespace tetris
