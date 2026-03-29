#include "include/ml/genetic.h"

#include "include/ml/neural-net.h"

#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <algorithm>
#include <atomic>

#define N_GAMES 3
#define MAX_PIECES 5000
// Extra fitness reward per tetris (4-line clear). This must be much larger than
// the raw game-score benefit (20 pts) so the GA strongly selects tetris-heavy agents
// instead of pure survival strategies.
#define TETRIS_FITNESS_BONUS 500

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
  const unsigned hw_threads = std::max(1U, std::thread::hardware_concurrency());
  const size_t worker_count = std::min(static_cast<size_t>(hw_threads), population_.agents_count());
  std::cout << "Using " << worker_count << " worker threads for training" << std::endl;

  while (true) {
    std::vector<brain_t> evaluated(population_.agents_count());
    std::atomic<size_t> next_agent {0};
    std::atomic<size_t> completed {0};
    const size_t progress_step = std::max<size_t>(1, population_.agents_count() / 10);

    std::vector<std::thread> workers;
    workers.reserve(worker_count);
    for (size_t t = 0; t < worker_count; t++) {
      workers.emplace_back([this, &evaluated, &next_agent, &completed, progress_step]() {
        while (true) {
          const size_t agent_idx = next_agent.fetch_add(1);
          if (agent_idx >= population_.agents_count()) {
            break;
          }

          evaluated[agent_idx] = run_games(population_.get_agents()[agent_idx]);

          const size_t done = completed.fetch_add(1) + 1;
          if (done % progress_step == 0 || done == population_.agents_count()) {
            std::lock_guard<std::mutex> lock(cout_mutex);
            std::cout << "gen " << gen_ << " progress " << done << "/" << population_.agents_count()
                      << "  current -> " << evaluated[agent_idx].get_score() << "  best -> " << best_recorded_fitnes_ << '\n';
          }
        }
      });
    }

    for (auto& worker : workers) {
      worker.join();
    }

    population_.get_agents() = std::move(evaluated);

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
    fitness += grid.get_score() + grid.get_tetrises() * TETRIS_FITNESS_BONUS;
  }

  brain_param.set_score(fitness / N_GAMES);
  return brain_param;
}

} // namespace ml
