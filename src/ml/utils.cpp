#include "include/ml/utils.h"

#include "include/ml/neural-net.h"

#include <cmath>
#include <fstream>
#include <ostream>
#include <random>

namespace {

std::random_device rd2;
std::uniform_int_distribution<int> d2(0, 10000000);

float get_random_param() {
  // returns random float between -1.0000000 and 1.0000000
  return static_cast<float>(d2(rd2)) / pow(10, 7) * (2 * (d2(rd2) % 2) - 1);
}

} // namespace

namespace ml {

float relu(float node) noexcept {
  return node > 0.0 ? node : 0.0;
}

float forward(const neural_net_t& net, const std::vector<int>& inputs) noexcept {
  float tmp = 0.0;
  float out = 0.0;

  for (int i = 0; i < NET_HIDDEN; i++) {
    tmp = 0.0;

    for (int j = 0; j < NET_INPUTS; j++) {
      tmp += inputs[j] * net.layer1[i][j];
    }

    out += relu(tmp + net.biases1[i]) * net.layer2[i];
  }

  return out + net.bias2;
}

void save(const neural_net_t& net, const std::string& filename, int score) noexcept {
  std::ofstream file(filename);
  file << "";
  file.close();
  std::ofstream fileAdd;
  fileAdd.open(filename, std::ios_base::app);

  for (int i = 0; i < NET_HIDDEN; i++) {
    for (int j = 0; j < NET_INPUTS; j++) {
      fileAdd << net.layer1[i][j] << '\n';
    }
    fileAdd << net.biases1[i] << '\n';
  }
  for (int i = 0; i < NET_HIDDEN; i++) {
    fileAdd << net.layer2[i] << '\n';
  }
  fileAdd << net.bias2 << '\n';

  fileAdd << score;
  fileAdd.close();
}

neural_net_t load(const std::string& filename) noexcept {
  neural_net_t net;
  char* line = nullptr;
  size_t len = 0;
  FILE* file = fopen(filename.c_str(), "r");

  for (int i = 0; i < NET_HIDDEN; i++) {
    std::vector<float> tmp;
    for (int j = 0; j < NET_INPUTS; j++) {
      getline(&line, &len, file);
      tmp.push_back(static_cast<float>(strtod(line, nullptr)));
    }
    net.layer1.push_back(tmp);
    getline(&line, &len, file);
    net.biases1.push_back(static_cast<float>(strtod(line, nullptr)));
  }
  for (int i = 0; i < NET_HIDDEN; i++) {
    getline(&line, &len, file);
    net.layer2.push_back(static_cast<float>(strtod(line, nullptr)));
  }
  getline(&line, &len, file);
  net.bias2 = static_cast<float>(strtod(line, nullptr));

  fclose(file);
  free(line);

  return net;
}

neural_net_t generate_net() noexcept {
  neural_net_t net;

  for (int i = 0; i < NET_HIDDEN; i++) {
    std::vector<float> tmp;
    for (int j = 0; j < NET_INPUTS; j++) {
      tmp.push_back(get_random_param());
    }
    net.layer1.push_back(tmp);
    net.biases1.push_back(get_random_param());
  }

  for (int i = 0; i < NET_HIDDEN; i++) {
    net.layer2.push_back(get_random_param());
  }
  net.bias2 = get_random_param();

  return net;
}

} // namespace ml
