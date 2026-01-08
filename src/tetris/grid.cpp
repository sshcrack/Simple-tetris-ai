#include "include/tetris/grid.h"
#include "include/tetris/constants.h"

#include <algorithm>

namespace {

bool inRange(int n, int a, int b) {
  return (a <= n && n < b);
}

bool contains(int n, const std::vector<int>& mat) {
  return (find(mat.begin(), mat.end(), n) != mat.end());
}

} // namespace

namespace tetris {

grid_t::grid_t() {
  game_over = false;
  cleared_lines = 0.0;
  score = 0;
  for (int i = 0; i < 24; i++) {
    matrix.push_back({0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
  }
}

void grid_t::clear_lines() {
  int nLines = 0;
  for (int j = 4; j < 24; j++) {
    if (!contains(0, matrix[j]) && !contains(1, matrix[j])) {
      for (int k = 1; k <= j; k++) {
        matrix[j - k + 1] = matrix[j - k];
      }
      matrix[0] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
      piece.set_position_row(piece.get_position()[0] + 1);
      cleared_lines++;
      nLines++;
    }
  }
  if (nLines == 1) {
    score += 1;
  } else if (nLines == 2) {
    score += 3;
  } else if (nLines == 3) {
    score += 5;
  } else if (nLines == 4) {
    score += 8;
  }
}

void grid_t::rotate_piece() {
  bool possible = true;
  int t = 0;

  while (t++ < 16 && possible) {
    int a = piece.get_position()[0] + (t / 4) + 1;
    int b = piece.get_position()[1] + (t % 4);

    if (((tetris::SHAPE_LIST[piece.get_n()][(piece.get_m() + 1) % 4] >> t) & 1)
        && (a > 23 || b < 0 || b > 9 || inRange(matrix[a][b], 2, 9))) {
      possible = false;
    }
  }

  if (possible) {
    piece.set_m((piece.get_m() + 1) % 4);
    piece.set_shape(tetris::SHAPE_LIST[piece.get_n()][piece.get_m()]);
  }
}

void grid_t::move_piece(int dir, int ind) {
  bool limit = false;
  int t = 0;

  while (t++ < 16 && !limit) {
    int a = piece.get_position()[0] + (t / 4);
    int b = piece.get_position()[1] + (t % 4);

    if ((piece.get_shape() >> t) & 1) {
      if (b == ind) {
        limit = true;
      } else if (0 <= b + dir && b + dir <= 9 && 0 <= a && a <= 23) {
        if ((a + 1 <= 23 && inRange(matrix[a + 1][b + dir], 2, 9)) || inRange(matrix[a][b + dir], 2, 9)) {
          limit = true;
        }
      }
    }
  }
  if (!limit) {
    piece.set_position_col(piece.get_position()[1] + dir);
  }
}

void grid_t::gravity(int val) {
  if (val == 1) {
    piece.set_position_row(piece.get_position()[0] + 1);
  } else if (val == 2) {
    gravity(1);
    update();
    gravity(1);
  } else {
    while (!piece.is_fixed()) {
      gravity(1);
      update();
    }
  }
}

void grid_t::fix_piece() {
  int b;
  int a = 0;
  piece.set_fixed(true);
  for (int k = 0; k < 16; k++) {
    a = piece.get_position()[0] + (k / 4);
    b = piece.get_position()[1] + (k % 4);

    if ((piece.get_shape() >> k) & 1) {
      if (a < 4) {
        game_over = true;
      } else if (a < 24) {
        matrix[a][b] = piece.get_n() + 2;
      }
    }
  }
  if (a < 4) {
    game_over = true;
  }
}

void grid_t::update() {
  bool fixed = false;
  int i = 0;

  for (int j = 0; j < 24; j++) {
    for (int l = 0; l < 10; l++) {
      if (matrix[j][l] == 1) {
        matrix[j][l] = 0;
      }
    }
  }

  while (i++ < 16 && !fixed) {
    int a = piece.get_position()[0] + (i / 4);
    int b = piece.get_position()[1] + (i % 4);

    if (((piece.get_shape() >> i) & 1) && 0 <= a && a < 24 && 0 <= b && b < 10) {
      if (a == 23 || inRange(matrix[a + 1][b], 2, 9)) {
        fixed = true;
        fix_piece();
      } else {
        matrix[a][b] = 1;
      }
    }
  }
}

} // namespace tetris
