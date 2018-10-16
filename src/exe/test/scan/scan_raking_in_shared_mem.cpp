#ifndef MULTIPLE_ELEMENTS
#define MULTIPLE_ELEMENTS 8
#endif
#include "scan.hpp"
#include <random>
int main() {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(1, 4);
  GLuint local_size = 1024;
#ifndef NO_SEQUENTIAL_ADDS
  GLuint reduction_factor = local_size * 2 * MULTIPLE_ELEMENTS;
#else
  GLuint reduction_factor = local_size * 2;
#endif
  GLuint block_size = 1024;
  GLuint numVectors = block_size * local_size * 2 * 2;
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
      std::begin(data.counter), std::prev(std::end(data.counter)), 0,
      std::plus<>(), [](const auto& elem) { return elem; });

  std::cout << "block value: " <<

      data.scan[numVectors / reduction_factor - 1].Scan_block_i << std::endl;
  auto sum_gpu = (*std::prev(std::end(data.scan))).Scan_local_i +
                 data.scan[numVectors / reduction_factor - 1].Scan_block_i;
  std::cout << "cpu reduction (last-1) elem: " << sum_cpu << std::endl;
  std::cout << "gpu scan last elem +last block elem:" << sum_gpu << std::endl;
  std::cout << "diff : " << sum_gpu - sum_cpu << std::endl;

  return 0;
}

