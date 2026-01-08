#include "include/tetris/piece.h"

namespace {

std::random_device rd1;
std::uniform_int_distribution<int> d1(0, 7);

} // namespace

namespace tetris {

std::vector<int> removeElement(std::vector<int> v, int elem) {
  int it = 0;
  while (v[it] != elem)
    it++;
  std::swap(v[it], v.back());
  v.pop_back();
  return v;
}

piece_t::piece_t() {
  pieceBag = {0, 1, 2, 3, 4, 5, 6};
  int s1 = pieceBag[d1(rd1) % pieceBag.size()];
  pieceBag = removeElement(pieceBag, s1);
  int s2 = pieceBag[d1(rd1) % pieceBag.size()];
  pieceBag = removeElement(pieceBag, s2);
  n = s1;
  next = s2;
  m = 0;
  position[0] = 0;
  position[1] = 3;
  fixed = false;
  std::vector<std::string> shape1 = {
          "        oo  oo  ", "        oo  oo  ", "        oo  oo  ", "        oo  oo  "}; // O
  std::vector<std::string> shape2 = {
          "    oooo        ", " o   o   o   o  ", "    oooo        ", " o   o   o   o  "}; // I
  std::vector<std::string> shape3 = {
          "    oo   oo     ", "  o  oo  o      ", "    oo   oo     ", "  o  oo  o      "}; // Z
  std::vector<std::string> shape4 = {
          "      oo oo     ", " o   oo   o     ", "      oo oo     ", " o   oo   o     "}; // S
  std::vector<std::string> shape5 = {
          "     o   o   oo ", "    ooo o       ", "     oo   o   o ", "      o ooo     "}; // L
  std::vector<std::string> shape6 = {
          "    o   ooo     ", "     oo  o   o  ", "    ooo   o     ", "      o   o  oo "}; // J
  std::vector<std::string> shape7 = {
          "    ooo  o      ", " o  oo   o      ", " o  ooo         ", " o   oo  o      "}; // T
  shapeList = {shape1, shape2, shape3, shape4, shape5, shape6, shape7};
  shapeRotations = {1, 2, 2, 2, 4, 4, 4};
  shape = shapeList[n][m];
}

const std::vector<int>& piece_t::get_piece_bag() const noexcept {
  return pieceBag;
}

void piece_t::set_piece_bag(const std::vector<int>& bag) noexcept {
  pieceBag = bag;
}

int piece_t::get_n() const noexcept {
  return n;
}

void piece_t::set_n(int value) noexcept {
  n = value;
}

int piece_t::get_m() const noexcept {
  return m;
}

void piece_t::set_m(int value) noexcept {
  m = value;
}

int piece_t::get_next() const noexcept {
  return next;
}

void piece_t::set_next(int value) noexcept {
  next = value;
}

const std::string& piece_t::get_shape() const noexcept {
  return shape;
}

void piece_t::set_shape(const std::string& value) noexcept {
  shape = value;
}

const int* piece_t::get_position() const noexcept {
  return position;
}

void piece_t::set_position(int row, int col) noexcept {
  position[0] = row;
  position[1] = col;
}

void piece_t::set_position_row(int row) noexcept {
  position[0] = row;
}

void piece_t::set_position_col(int col) noexcept {
  position[1] = col;
}

bool piece_t::is_fixed() const noexcept {
  return fixed;
}

void piece_t::set_fixed(bool value) noexcept {
  fixed = value;
}

const std::vector<std::vector<std::string>>& piece_t::get_shape_list() const noexcept {
  return shapeList;
}

const std::vector<int>& piece_t::get_shape_rotations() const noexcept {
  return shapeRotations;
}

void piece_t::new_shape() {
  n = next;
  m = 0;
  shape = shapeList[n][m];
  position[0] = 0;
  position[1] = 3;
  fixed = false;
}

void piece_t::new_next() {
  int s = pieceBag[d1(rd1) % pieceBag.size()];
  pieceBag = removeElement(pieceBag, s);
  next = s;
  if (pieceBag.size() == 0)
    pieceBag = {0, 1, 2, 3, 4, 5, 6};
}

} // namespace tetris
