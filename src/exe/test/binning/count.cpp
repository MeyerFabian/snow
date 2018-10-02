#ifndef GLM_INCLUDE_GUARD
#define GLM_INCLUDE_GUARD
#include <glm/gtc/random.hpp>
#include <numeric>
#include "../../../../shader/shared_hpp/buffer_bindings.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>
#endif
#ifndef UNARY_OPERATION_GL
#include "../../src/snow/particle/particle_exp.hpp"

#define BUFFER_IN_NAME "shader/buffers/particle_system.include.glsl"
#define BUFFER_OUT_NAME "shader/buffers/atomic_counter.include.glsl"
#endif

#include <execution>
#include "../../../snow/shader/shader.hpp"
#include "../../../snow/utils/benchmarker.hpp"
#include "../../../test/binning/countTechnique.hpp"
#include "../../../test/test_util.hpp"
int main() {
  GLFWWindow();
  GLuint numVectors = 1024 * 1024;
  std::vector<Particle_exp> particles;
  std::vector<GLuint> grid_counter;
  for (size_t i = 0; i < numVectors; i++) {
    particles.push_back(
        {PREC_VEC_TYPE(glm::linearRand(0.0f, 1.0f), glm::linearRand(0.0f, 1.0f),
                       glm::linearRand(0.0f, 1.0f), 0.0f)

        });
    grid_counter.push_back({0});
  }
#ifdef AOS_LAYOUT
  BufferLayout layout = BufferLayout::AOS;
#else
  BufferLayout layout = BufferLayout::SOA;
#endif
  Buffer<Particle_exp> input(BufferType::SSBO, BufferUsage::STATIC_DRAW, layout,
                             BUFFER_IN_NAME);

  input.transfer_to_gpu(particles);
  input.gl_bind_base(PARTICLE_SYSTEM_BINDING);

  Buffer<GLuint> output(BufferType::SSBO, BufferUsage::STATIC_DRAW, layout,
                        BUFFER_OUT_NAME);
  output.transfer_to_gpu(grid_counter);
  output.gl_bind_base(ATOMIC_COUNTER_BINDING);
  CountingTechnique::CountingData counting_data({
      // IOBufferData
      //   In
      "particles",
      "PARTICLE_pos_mass",
      input.get_buffer_info(),
      "Counter",
      "Counter_i",
      output.get_buffer_info(),
      // numVectors
      numVectors,
  });

  auto test = CountingTechnique();
  test.init(std::move(counting_data));
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

  auto m(output.transfer_to_cpu(std::size(grid_counter)));
  BenchmarkerGPU::write_to_file("Counting");
  bench.write_to_file("CountingCPU");

  auto sum_gpu =
      std::transform_reduce(std::begin(m), std::end(m), 0.0f, std::plus<>(),
                            [](const auto& elem) { return elem; });
  std::cout << "GPU map, CPU sum: " << sum_gpu << std::endl;
  std::cout << "Difference: " << std::abs(std::size(particles) - sum_gpu)
            << std::endl;

  GLFWWindow::swapBuffers();

  GLFWWindow::stop();

  return 0;
}

