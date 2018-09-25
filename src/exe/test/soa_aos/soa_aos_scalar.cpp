#define AOS_LAYOUT
#include <execution>
#include <glm/gtc/random.hpp>
#include <numeric>
#include "../../../snow/rendering/GLFWWindow.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>
#include "../../../../shader/shared_hpp/precision.hpp"
#include "../../../snow/shader/shader.hpp"
#include "../../../snow/utils/benchmarker.hpp"
#include "../../../test/map/mapTechnique.hpp"
#include "../../../test/soa_aos/gpu_in_out_structs_scalar.hpp"
#include "../../../test/test_util.hpp"
int main() {
  GLFWWindow();
  size_t numVectors = 1'024 * 1'024;
  std::vector<Input> input_data;
  std::vector<Output> output_data_init;
  for (size_t i = 0; i < numVectors; i++) {
    input_data.push_back({PREC_SCAL_TYPE(1.0f), PREC_SCAL_TYPE(1.0f)});
    // val here (50.0f) is actually overwritten, so this is another test
    output_data_init.push_back({0.0f, 0.0f});
  }

  Buffer<Input> input(BufferType::SSBO, BufferUsage::STATIC_DRAW,
                      BufferLayout::AOS);
  input.transfer_to_gpu(input_data);
  input.gl_bind_base(1);

  Buffer<Output> output(BufferType::SSBO, BufferUsage::STATIC_DRAW,
                        BufferLayout::AOS);
  output.transfer_to_gpu(output_data_init);
  output.gl_bind_base(2);
  MapTechnique::MapData map_data({
      "length(value)",
      "g_in",
      "g_out",
      "v",
      "g",
      "shader/test/soa_aos/buffer_scalar.include.glsl",
  });

  auto test = MapTechnique();
  test.init(std::move(map_data));
  BenchmarkerCPU bench;
  bench.time("Total CPU time spent", [&numVectors, &test]() {
    executeTest(1'000, [&test, numVectors]() {
      return BenchmarkerGPU::getInstance().time("map", [&test, numVectors]() {
        test.dispatch_with_barrier(numVectors);
      });
    });
  });

  BenchmarkerGPU::getInstance().collect_times_last_frame();
  BenchmarkerGPU::getInstance().collect_times_last_frame();

  auto m(output.transfer_to_cpu(std::size(output_data_init)));

  BenchmarkerGPU::write_to_file("Map");
  bench.write_to_file("MapCPU");
  auto sum = std::transform_reduce(std::begin(input_data), std::end(input_data),
                                   0.0f, std::plus<>(),
                                   [](const auto& elem) { return elem.v; });

  auto sum_gpu =
      std::transform_reduce(std::begin(m), std::end(m), 0.0f, std::plus<>(),
                            [](const auto& elem) { return elem.g; });
  std::cout << "CPU map, CPU sum: " << sum << std::endl;
  std::cout << "GPU map, CPU sum: " << sum_gpu << std::endl;
  std::cout << "Difference: " << std::abs(sum - sum_gpu) << std::endl;

  GLFWWindow::swapBuffers();

  GLFWWindow::stop();

  return 0;
}

