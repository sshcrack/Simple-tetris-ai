#include "include/ml/brain.h"
#include "include/ml/neural-net.h"
#include "include/ml/utils.h"
#include "include/tetris/grid.h"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

static constexpr int N_GAMES = 20;
static constexpr int MAX_PIECES = 5000;

struct GameResult {
  int score;
  int lines;
  int tetrises;
  int pieces;
};

GameResult run_one_game(ml::brain_t& brain) {
  tetris::grid_t grid;
  int pieces = 0;
  while (!grid.is_game_over() && pieces++ < MAX_PIECES) {
    std::string best_move = brain.best_move(grid);
    int best_rotation = static_cast<int>(best_move.back()) - 48;
    best_move.pop_back();

    for (int i = 0; i < best_rotation; i++) {
      grid.rotate_piece();
    }

    if (best_move[0] == 'r') {
      for (size_t m = 0; m < best_move.size(); m++) {
        grid.move_piece(1, 9);
      }
    } else {
      for (size_t m = 0; m < best_move.size(); m++) {
        grid.move_piece(-1, 0);
      }
    }

    while (!grid.get_piece().is_fixed()) {
      grid.gravity(1);
      grid.clear_lines();
      grid.update();
    }

    grid.get_piece().new_shape();
    grid.get_piece().new_next();
  }
  return {grid.get_score(), static_cast<int>(grid.get_cleared_lines()), grid.get_tetrises(), pieces};
}

int main() {
  ml::neural_net_t params = ml::load("data/bestParams.txt");
  ml::brain_t brain(params);

  std::vector<GameResult> results;
  results.reserve(N_GAMES);

  std::cout << "Running " << N_GAMES << " games...\n";
  for (int g = 0; g < N_GAMES; g++) {
    GameResult r = run_one_game(brain);
    results.push_back(r);
    float tetris_rate = (r.tetrises > 0 && r.lines > 0)
                          ? static_cast<float>(r.tetrises * 4) / r.lines * 100.0f
                          : 0.0f;
    std::cout << "game " << std::setw(2) << (g + 1)
              << " -> score: " << std::setw(6) << r.score
              << "  lines: " << std::setw(5) << r.lines
              << "  tetrises: " << std::setw(4) << r.tetrises
              << "  tetris%: " << std::fixed << std::setprecision(1) << tetris_rate << "%"
              << "  pieces: " << r.pieces << "\n";
  }

  // Compute statistics
  double avg_score = 0, avg_lines = 0, avg_tetrises = 0, avg_pieces = 0;
  double avg_tetris_rate = 0;
  int max_score = 0, max_lines = 0, max_tetrises = 0;
  for (const auto& r : results) {
    avg_score += r.score;
    avg_lines += r.lines;
    avg_tetrises += r.tetrises;
    avg_pieces += r.pieces;
    if (r.lines > 0) {
      avg_tetris_rate += static_cast<double>(r.tetrises * 4) / r.lines * 100.0;
    }
    if (r.score > max_score) max_score = r.score;
    if (r.lines > max_lines) max_lines = r.lines;
    if (r.tetrises > max_tetrises) max_tetrises = r.tetrises;
  }
  avg_score /= N_GAMES;
  avg_lines /= N_GAMES;
  avg_tetrises /= N_GAMES;
  avg_pieces /= N_GAMES;
  avg_tetris_rate /= N_GAMES;

  // Compute std dev for score
  double var_score = 0;
  for (const auto& r : results) {
    var_score += (r.score - avg_score) * (r.score - avg_score);
  }
  var_score /= N_GAMES;

  std::cout << "\n========== BENCHMARK RESULTS (" << N_GAMES << " games) ==========\n";
  std::cout << std::fixed << std::setprecision(2);
  std::cout << "Average score:         " << avg_score << " (±" << std::sqrt(var_score) << ")\n";
  std::cout << "Average lines cleared: " << avg_lines << "\n";
  std::cout << "Average tetrises:      " << avg_tetrises << "\n";
  std::cout << "Average tetris rate:   " << avg_tetris_rate << "% of lines via tetris\n";
  std::cout << "Average pieces placed: " << avg_pieces << "\n";
  std::cout << "Max score:             " << max_score << "\n";
  std::cout << "Max lines:             " << max_lines << "\n";
  std::cout << "Max tetrises:          " << max_tetrises << "\n";

  return 0;
}
