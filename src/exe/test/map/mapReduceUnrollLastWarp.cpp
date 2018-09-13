#include "../../../test/map/mapReduceTechnique.hpp"
//#undef MARKERS  // NVIDIA NSIGHT
#include "../../../test/map/mapReduceBuffers.hpp"
#include "../../../test/map/mapReducePipeline.hpp"
#include "../../../test/test_util.hpp"
int main() {
  GLFWWindow();

  GLuint numVectors = 1024 * 1024;
  LocalSize local_size = {1024, 1, 1};

  auto buffer = MapReduceBuffers(numVectors, local_size);
  MapReducePipeline test;
  test.init({
      "shader/test/map/mapReduceUnrollLastWarp.glsl",
      local_size,
      "length(value)",
      "left+right",
  });
  float sum_gpu;
  BenchmarkerCPU bench;
  bench.time("Total CPU time spent", [&buffer, numVectors, &test, &sum_gpu]() {
    executeTest(100'000,
                [&test, numVectors]() { return test.run(numVectors); });

    sum_gpu = test.fetch_gpu_result(numVectors, buffer.output,
                                    [](const auto& elem) { return elem.f; },
                                    std::plus<>());
    BenchmarkerGPU::getInstance().collect_times_last_frame();
  });

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
  bench.write_to_file("CPU time");
  // test.print();

  GLFWWindow::stop();
  return 0;
}

