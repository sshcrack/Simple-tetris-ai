#include "include/tetris/piece.h"
#include "include/tetris/constants.h"

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
  int s = pieceBag[d1(rd1) % pieceBag.size()];
  pieceBag = removeElement(pieceBag, s);
  next = s;
  if (pieceBag.size() == 0)
    pieceBag = {0, 1, 2, 3, 4, 5, 6};
}

} // namespace tetris
