#include "scan.hpp"
#include <random>
int main() {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(1, 4);
  GLuint numVectors = 128;
  std::vector<GLuint> counter;
  for (size_t i = 0; i < numVectors; i++) {
    counter.push_back({
        (GLuint)distribution(generator),
    });
  }

  std::vector<GLuint> scan;
  for (size_t i = 0; i < numVectors; i++) {
    scan.push_back(0);
  }
  testData data = {numVectors, counter, scan};

  test(data);
  int diff = 0;
  int i = 0;
  for (auto it_pair = std::make_pair<decltype(std::begin(data.scan)),
                                     decltype(std::begin(data.counter))>(
           std::begin(data.scan), std::begin(data.counter));
       it_pair.first != std::end(data.scan);
       it_pair.first++, it_pair.second++) {
    // diff += (int)*(std::prev(it_pair.first)) + (int)(*it_pair.second)
    // -(int)(*it_pair.first);
    std::cout << i << ": " << *it_pair.second << " " << *it_pair.first
              << std::endl;
    i++;
  }

  auto sum_gpu = std::transform_reduce(
      std::begin(data.counter), std::prev(std::end(data.counter)), 0.0f,
      std::plus<>(), [](const auto& elem) { return elem; });

  std::cout << "Last element transform_reduce" << sum_gpu << std::endl;
  std::cout << "Last element scan:" << *(std::prev(std::end(data.scan)))
            << std::endl;

  std::cout << "GPU map, CPU sum: " << diff << std::endl;
  return 0;
}

