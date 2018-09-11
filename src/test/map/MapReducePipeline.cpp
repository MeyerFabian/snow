#include "mapReduce.hpp"
int main() {
  GLFWWindow();

  size_t numVectors = 1'024 * 1'024;
  LocalSize local_size = {1024, 1, 1};

  auto buffer = MapReduceBuffers(numVectors, local_size);
  MapReducePipeline test;
  test.init({
      "shader/compute/mapreduce/mapReduce.glsl",
      local_size,
      "length(value)",
      "left+right",
  });
  float sum_gpu;
#ifdef MARKERS
  while (GLFWWindow::shouldClose()) {
#endif
    test.run(numVectors);
    GLFWWindow::clear();
    GLFWWindow::swapBuffers();
    sum_gpu = test.fetch_gpu_result(numVectors, buffer.output,
                                    [](const auto& elem) { return elem.f; },
                                    std::plus<>());

    BenchmarkerGPU::getInstance().collect_times_last_frame();
#ifdef MARKERS
  }
#endif
  auto sum = std::transform_reduce(
      std::execution::par, std::begin(buffer.input_data),
      std::end(buffer.input_data), 0.0f, std::plus<>(),
      [](const auto& elem) { return glm::l2Norm(glm::vec3(elem.v)); });
  std::cout << "CPU map, CPU sum: " << sum << std::endl;
  std::cout << "GPU map, GPU sum: " << sum_gpu << std::endl;
  float abs_error = std::abs(sum - sum_gpu);
  float rel_error = abs_error / sum;
  std::cout << "Absolute error: " << abs_error << std::endl;
  std::cout << "Relative error: " << rel_error << std::endl;

  BenchmarkerGPU::getInstance().collect_times_last_frame();
  BenchmarkerGPU::write_to_file("MapReduce");
  // test.print();

  GLFWWindow::stop();
  return 0;
}

