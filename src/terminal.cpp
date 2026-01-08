#include "include/ml/brain.h"
#include "include/ml/neural-net.h"
#include "include/ml/utils.h"
#include "include/tetris/grid.h"
#include "include/tetris/piece.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>

int main() {

  char* line = nullptr;
  size_t len = 0;
  int l = 1;
  ml::neural_net_t bestParams = ml::load("data/bestParams.txt");
  FILE* file = fopen("data/record.txt", "r");
  int record;
  bool newRecord = false;

  if (getline(&line, &len, file) != -1)
    record = atoi(line);
  fclose(file);
  free(line);

  tetris::grid_t grid;
  ml::brain_t brain(bestParams);
  std::string bestM;
  std::string bestMove = brain.best_move(grid);
  int bestRotation = (int)bestMove.back() - 48;
  bestMove.pop_back();
  for (int i = 0; i < bestRotation; i++)
    grid.rotate_piece();

  while (!grid.is_game_over()) {

    if (bestMove.size() > 0) {
      if (bestMove[0] == 'r') {
        grid.move_piece(1, 9);
        bestMove.pop_back();
      } else {
        grid.move_piece(-1, 0);
        bestMove.pop_back();
      }
    }

    grid.gravity(1);
    grid.clear_lines();
    grid.update();

    if (grid.get_piece().is_fixed()) {
      grid.get_piece().new_shape();
      grid.get_piece().new_next();
      bestMove = brain.best_move(grid);
      bestRotation = (int)bestMove.back() - 48;
      bestMove.pop_back();
      bestM = bestMove;
      for (int i = 0; i < bestRotation; i++)
        grid.rotate_piece();
    }

    if (newRecord)
      std::cout << "NEW RECORD" << '\n';
    std::cout << "cleared lines -> " << grid.get_cleared_lines() << '\n';
    std::cout << "record -> " << record << '\n' << '\n';
    if (bestM[0] == 'r')
      std::cout << "best move -> " << bestM.size() << " time(s) right and " << bestRotation << " rotation(s)" << '\n';
    else if (bestM[0] == 'l')
      std::cout << "best move -> " << bestM.size() << " time(s) left and " << bestRotation << " rotation(s)" << '\n';
    else
      std::cout << "best move -> don't move and " << bestRotation << " rotation(s)" << '\n';
    std::cout << '\n';
    for (int j = 4; j < 24; j++) {
      for (int i = 0; i < 10; i++) {
        if (grid.get_matrix()[j][i] == 0) {
          std::cout << "· ";
        } else {
          std::cout << grid.get_matrix()[j][i] << " ";
        }
      }
      std::cout << '\n';
    }
    std::cout << '\n' << '\n';

    if (grid.get_cleared_lines() > record) {
      newRecord = true;
      std::ofstream file("data/record.txt");
      if (file.is_open()) {
        record = grid.get_cleared_lines();
        file << record;
        file.close();
      }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
  }
  return 0;
}
