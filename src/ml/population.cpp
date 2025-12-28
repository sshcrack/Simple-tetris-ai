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
  std::vector<brain_t> newOffsprings;
  int nWorstAgents = NUM_AGENTS * 0.3;
  int nSelection = NUM_AGENTS * 0.1;

  for (int i = 0; i < nWorstAgents; i++) {
    std::vector<brain_t> randomSelection;

    for (int j = 0; j < nSelection; j++) {
      randomSelection.push_back(agents_[d3(rd3) % NUM_AGENTS]);
    }

    sort(randomSelection.begin(), randomSelection.end(), [](brain_t& b1, brain_t& b2) -> bool {
      return b1.get_score() < b2.get_score();
    });

    brain_t newbrain_t = randomSelection[nSelection - 1].crossover(randomSelection[nSelection - 2]);
    newOffsprings.push_back(newbrain_t);
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

  for (int i = 0; i < nWorstAgents; i++)
    agents_[i] = newOffsprings[i];
}

} // namespace ml
