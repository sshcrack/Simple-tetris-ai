#include "include/ml/utils.h"

#include "include/ml/neural-net.h"

#include <cmath>
#include <fstream>
#include <ostream>
#include <random>

namespace {

std::random_device rd2;
std::uniform_int_distribution<int> d2(0, 10000000);

float getRandomParam() {
  // returns random float between -1.0000000 and 1.0000000
  return (float)d2(rd2) / pow(10, 7) * (2 * (d2(rd2) % 2) - 1);
}

} // namespace

namespace ml {

float relu(float node) noexcept {
  return node > 0.0 ? node : 0.0;
}

float forward(const neural_net_t& net, const std::vector<int>& inputs) noexcept {
  float tmp, out = 0.0;

  for (int i = 0; i < 3; i++) {
    tmp = 0.0;

    for (int j = 0; j < 4; j++) {
      tmp += inputs[j] * net.layer1[i][j];
    }

    out += relu(tmp + net.biases1[i]) * net.layer2[i];
  }

  return out + net.bias2;
}

void save(const neural_net_t& net, const std::string& filename, int score) noexcept {
  int i, j;
  std::ofstream file(filename);
  file << "";
  file.close();
  std::ofstream fileAdd;
  fileAdd.open(filename, std::ios_base::app);

  for (i = 0; i < 3; i++) {
    for (j = 0; j < 4; j++) {
      fileAdd << net.layer1[i][j] << '\n';
    }
    fileAdd << net.biases1[i] << '\n';
  }
  for (i = 0; i < 3; i++) {
    fileAdd << net.layer2[i] << '\n';
  }
  fileAdd << net.bias2 << '\n';

  fileAdd << score;
  fileAdd.close();
}

neural_net_t load(const std::string& filename) noexcept {
  neural_net_t net;
  int i, j;
  char* line = nullptr;
  size_t len = 0;
  FILE* file = fopen(filename.c_str(), "r");

  for (i = 0; i < 3; i++) {
    std::vector<float> tmp;
    for (j = 0; j < 4; j++) {
      getline(&line, &len, file);
      tmp.push_back((float)strtod(line, NULL));
    }
    net.layer1.push_back(tmp);
    getline(&line, &len, file);
    net.biases1.push_back((float)strtod(line, NULL));
  }
  for (i = 0; i < 3; i++) {
    getline(&line, &len, file);
    net.layer2.push_back((float)strtod(line, NULL));
  }
  getline(&line, &len, file);
  net.bias2 = (float)strtod(line, NULL);

  fclose(file);
  free(line);

  return net;
}

neural_net_t generate_net() noexcept {
  neural_net_t net;

  for (int i = 0; i < 3; i++) {
    std::vector<float> tmp;
    for (int j = 0; j < 4; j++) {
      tmp.push_back(getRandomParam());
    }
    net.layer1.push_back(tmp);
    net.biases1.push_back(getRandomParam());
  }

  for (int i = 0; i < 3; i++) {
    net.layer2.push_back(getRandomParam());
  }
  net.bias2 = getRandomParam();

  return net;
}

} // namespace ml
