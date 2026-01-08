#include "include/ml/genetic.h"

#include <iostream>
#include <string>

#define N_GAMES 3

namespace ml {

genetic_t::genetic_t() {
  gen_ = 1;
  int l = 0;
  char* line = nullptr;
  size_t len = 0;
  FILE* file1 = fopen("data/bestParams.txt", "r");
  while (getline(&line, &len, file1) != -1) {
    if (l++ == 4 * 3 + 3 + 3 + 1) {
      best_recorded_fitnes_ = static_cast<int>(strtol(line, nullptr, 10));
    }
  }
  fclose(file1);
  free(line);
}

void genetic_t::fit() {
  while (true) {
    int agent = 0;
    float best_fitnes = population_.get_agents()[0].get_score();

    for (size_t b = 0; b < population_.agents_count(); b++) {
      std::cout << "gen " << gen_ << "   agent " << ++agent << "/" << population_.agents_count() << '\n';
      population_.get_agents()[b] = run_games(population_.get_agents()[b]);
      std::cout << "current -> " << population_.get_agents()[b].get_score() << "  best -> " << best_fitnes << '\n';
    }
    if (population_.get_agents().back().get_score() > best_recorded_fitnes_) {
      best_recorded_fitnes_ = population_.get_agents().back().get_score();
      population_.get_agents().back().save("data/bestParams.txt");
    }

    std::cout << '\n' << '\n' << "GENERATION " << gen_++ << '\n' << '\n';
    population_.generate_new();
  }
}

brain_t genetic_t::run_games(brain_t brain_param) {
  int fitness = 0;

  for (int g = 0; g < N_GAMES; g++) {
    tetris::grid_t grid;
    int pieces = 0;
    while (!grid.is_game_over() && pieces++ < 50000) {
      std::string best_move = brain_param.best_move(grid);
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
    std::cout << "game " << g + 1 << " ->  score: " << grid.get_score() << "  lines: " << grid.get_cleared_lines() << '\n';
    fitness += grid.get_score();
  }

  brain_param.set_score(fitness / N_GAMES);
  return brain_param;
}

} // namespace ml
