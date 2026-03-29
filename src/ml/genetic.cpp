#include "include/ml/genetic.h"

#include "include/ml/neural-net.h"

#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <future>
#include <algorithm>

#define N_GAMES 3
#define MAX_PIECES 20000

static std::mutex cout_mutex;

namespace ml {

genetic_t::genetic_t() {
  gen_ = 1;
  int l = 0;
  char* line = nullptr;
  size_t len = 0;
  FILE* file1 = fopen("data/bestParams.txt", "r");
  while (getline(&line, &len, file1) != -1) {
    if (l++ == NET_INPUTS * NET_HIDDEN + NET_HIDDEN + NET_HIDDEN + 1) {
      best_recorded_fitnes_ = static_cast<int>(strtol(line, nullptr, 10));
    }
  }
  fclose(file1);
  free(line);
}

void genetic_t::fit() {
  const unsigned num_threads = std::thread::hardware_concurrency();
  std::cout << "Using " << num_threads << " threads for training" << std::endl;

  while (true) {
    float best_fitnes = population_.get_agents()[0].get_score();

    std::vector<std::future<std::pair<size_t, brain_t>>> futures;

    for (size_t b = 0; b < population_.agents_count(); b++) {
      futures.push_back(std::async(std::launch::async, [this, b, best_fitnes]() {
        {
          std::lock_guard<std::mutex> lock(cout_mutex);
          std::cout << "gen " << gen_ << "   agent " << (b + 1) << "/" << population_.agents_count() << '\n';
        }
        brain_t result = run_games(population_.get_agents()[b]);
        {
          std::lock_guard<std::mutex> lock(cout_mutex);
          std::cout << "current -> " << result.get_score() << "  best -> " << best_fitnes << '\n';
        }
        return std::make_pair(b, result);
      }));

      if (futures.size() >= num_threads || b == population_.agents_count() - 1) {
        for (auto& f : futures) {
          auto [agent_idx, result] = f.get();
          population_.get_agents()[agent_idx] = result;
        }
        futures.clear();
      }
    }

    auto best_agent = std::max_element(population_.get_agents().begin(), population_.get_agents().end(),
      [](const brain_t& b1, const brain_t& b2) { return b1.get_score() < b2.get_score(); });

    if (best_agent->get_score() > best_recorded_fitnes_) {
      best_recorded_fitnes_ = best_agent->get_score();
      best_agent->save("data/bestParams.txt");
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
    while (!grid.is_game_over() && pieces++ < MAX_PIECES) {
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
    {
      std::lock_guard<std::mutex> lock(cout_mutex);
      std::cout << "game " << g + 1 << " ->  score: " << grid.get_score() << "  lines: " << grid.get_cleared_lines() << "  tetrises: " << grid.get_tetrises() << '\n';
    }
    fitness += grid.get_score();
  }

  brain_param.set_score(fitness / N_GAMES);
  return brain_param;
}

} // namespace ml
