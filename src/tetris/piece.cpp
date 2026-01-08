#include "include/tetris/piece.h"

namespace {

std::random_device rd1;
std::uniform_int_distribution<int> d1(0, 7);

std::vector<int> removeElement(std::vector<int> v, int elem) {
  int it = 0;
  while (v[it] != elem)
    it++;
  std::swap(v[it], v.back());
  v.pop_back();
  return v;
}

uint16_t string_to_bits(const char* s) {
  uint16_t result = 0;
  for (int i = 0; i < 16; i++) {
    if (s[i] == 'o') {
      result |= (1U << i);
    }
  }
  return result;
}

} // namespace

namespace tetris {

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

  // O piece - 2x2 square (all rotations same)
  std::vector<uint16_t> shape1 = {
      string_to_bits("        oo  oo  "),
      string_to_bits("        oo  oo  "),
      string_to_bits("        oo  oo  "),
      string_to_bits("        oo  oo  ")
  };

  // I piece - 4x1 line
  std::vector<uint16_t> shape2 = {
      string_to_bits("    oooo        "),
      string_to_bits(" o   o   o   o  "),
      string_to_bits("    oooo        "),
      string_to_bits(" o   o   o   o  ")
  };

  // Z piece
  std::vector<uint16_t> shape3 = {
      string_to_bits("    oo   oo     "),
      string_to_bits("  o  oo  o      "),
      string_to_bits("    oo   oo     "),
      string_to_bits("  o  oo  o      ")
  };

  // S piece
  std::vector<uint16_t> shape4 = {
      string_to_bits("      oo oo     "),
      string_to_bits(" o   oo   o     "),
      string_to_bits("      oo oo     "),
      string_to_bits(" o   oo   o     ")
  };

  // L piece
  std::vector<uint16_t> shape5 = {
      string_to_bits("     o   o   oo "),
      string_to_bits("    ooo o       "),
      string_to_bits("     oo   o   o "),
      string_to_bits("      o ooo     ")
  };

  // J piece
  std::vector<uint16_t> shape6 = {
      string_to_bits("    o   ooo     "),
      string_to_bits("     oo  o   o  "),
      string_to_bits("    ooo   o     "),
      string_to_bits("      o   o  oo ")
  };

  // T piece
  std::vector<uint16_t> shape7 = {
      string_to_bits("    ooo  o      "),
      string_to_bits(" o  oo   o      "),
      string_to_bits(" o  ooo         "),
      string_to_bits(" o   oo  o      ")
  };

  shapeList = {shape1, shape2, shape3, shape4, shape5, shape6, shape7};
  shapeRotations = {1, 2, 2, 2, 4, 4, 4};
  shape = shapeList[n][m];
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
