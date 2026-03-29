#include "include/tetris/piece.h"
#include "include/tetris/constants.h"

namespace {

thread_local std::mt19937 rng(std::random_device{}());

std::vector<int> removeElement(std::vector<int> v, int elem) {
  int it = 0;
  while (v[it] != elem) {
    it++;
  }
  std::swap(v[it], v.back());
  v.pop_back();
  return v;
}

} // namespace

namespace {

int random_bag_index(size_t bag_size) {
  std::uniform_int_distribution<int> dist(0, static_cast<int>(bag_size) - 1);
  return dist(rng);
}

} // namespace

namespace tetris {

piece_t::piece_t() {
  piece_bag = {0, 1, 2, 3, 4, 5, 6};
  int s1 = piece_bag[random_bag_index(piece_bag.size())];
  piece_bag = removeElement(piece_bag, s1);
  int s2 = piece_bag[random_bag_index(piece_bag.size())];
  piece_bag = removeElement(piece_bag, s2);
  n = s1;
  next = s2;
  m = 0;
  position[0] = 0;
  position[1] = 3;
  fixed = false;

  shape = SHAPE_LIST[n][m];
}

void piece_t::new_shape() {
  n = next;
  m = 0;
  shape = SHAPE_LIST[n][m];
  position[0] = 0;
  position[1] = 3;
  fixed = false;
}

void piece_t::new_next() {
  int s = piece_bag[random_bag_index(piece_bag.size())];
  piece_bag = removeElement(piece_bag, s);
  next = s;
  if (piece_bag.size() == 0)
    piece_bag = {0, 1, 2, 3, 4, 5, 6};
}

} // namespace tetris
