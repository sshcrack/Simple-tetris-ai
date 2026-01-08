#include "include/ml/population.h"
#include "include/ml/utils.h"

#include <algorithm>
#include <iostream>

#define NUM_AGENTS 300

namespace {

std::random_device rd3;
std::uniform_int_distribution<int> d3(0, NUM_AGENTS);

} // namespace

namespace ml {

population_t::population_t() {
  agents_.reserve(NUM_AGENTS);
  for (int a = 0; a < NUM_AGENTS; a++) {
    agents_.emplace_back(generate_net());
  }
}

void population_t::generate_new() {
  std::vector<brain_t> new_offsprings;
  int n_worst_agents = NUM_AGENTS * 0.3;
  int n_selection = NUM_AGENTS * 0.1;

  for (int i = 0; i < n_worst_agents; i++) {
    std::vector<brain_t> random_selection;

    for (int j = 0; j < n_selection; j++) {
      random_selection.push_back(agents_[d3(rd3) % NUM_AGENTS]);
    }

    sort(random_selection.begin(), random_selection.end(), [](brain_t& b1, brain_t& b2) -> bool {
      return b1.get_score() < b2.get_score();
    });

    brain_t new_brain_t = random_selection[n_selection - 1].crossover(random_selection[n_selection - 2]);
    new_offsprings.push_back(new_brain_t);
  }
  sort(agents_.begin(), agents_.end(), [](brain_t& b1, brain_t& b2) -> bool {
    return b1.get_score() < b2.get_score();
  });

  int totalFitness = 0;
  for (int a = 0; a < NUM_AGENTS; a++)
    totalFitness += agents_[a].get_score();

  std::cout << "#############################################################" << '\n' << '\n';
  std::cout << "average fitness: " << (float)totalFitness / NUM_AGENTS << '\n';
  std::cout << "highest fitness: " << agents_.back().get_score() << '\n';
  std::cout << '\n' << "#############################################################" << '\n' << '\n' << '\n';

  for (int i = 0; i < n_worst_agents; i++)
    agents_[i] = new_offsprings[i];
}

} // namespace ml
