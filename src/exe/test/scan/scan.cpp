#include "scan.hpp"
#include <random>
int main() {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(1, 4);
  GLuint local_size = 1024;
  GLuint block_size = 16 * (4 * 2);

  GLuint numVectors = block_size * 4 * 1024 * 2;
  std::vector<GLuint> counter;
  for (size_t i = 0; i < numVectors; i++) {
    counter.push_back({
        (GLuint)distribution(generator),
    });
  }

  std::vector<Scan> scan;
  for (size_t i = 0; i < numVectors; i++) {
    scan.push_back({0, 0});
  }
  testData data = {{local_size, 1, 1}, numVectors, counter, scan};

  test(data);
  auto sum_cpu = std::transform_reduce(
      std::begin(data.counter), std::prev(std::end(data.counter)), 0.0f,
      std::plus<>(), [](const auto& elem) { return elem; });

  auto sum_gpu = (*std::prev(std::end(data.scan))).Scan_local_i +
                 data.scan[block_size - 1].Scan_block_i;
  std::cout << "last block " << data.scan[block_size - 1].Scan_block_i;
  std::cout << "cpu reduction (last-1) elem: " << sum_cpu << std::endl;
  std::cout << "gpu scan last elem +last block elem:" << sum_gpu << std::endl;
  std::cout << "diff : " << sum_gpu - sum_cpu << std::endl;
  return 0;
}

