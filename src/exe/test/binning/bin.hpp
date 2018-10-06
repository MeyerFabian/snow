#ifndef GLM_INCLUDE_GUARD
#define GLM_INCLUDE_GUARD
#include "../../../snow/utils/defines.hpp"

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
#include "../../../test/map/mapTechnique.hpp"
#include "../../../test/test_util.hpp"

struct testData {
  GLuint numVectors;
  GLuint numGridPoints;
  std::vector<Particle_exp> particles;
  std::vector<GLuint> counter;
  glm::uvec3 gGridDim;
  PREC_VEC3_TYPE gGridPos;
  PREC_SCAL_TYPE gridSpacing;
};

std::vector<GLuint> test(testData& data) {
  GLFWWindow();
#ifdef AOS_LAYOUT
  BufferLayout layout = BufferLayout::AOS;
#else
  BufferLayout layout = BufferLayout::SOA;
#endif
  Buffer<Particle_exp> input(BufferType::SSBO, BufferUsage::STATIC_DRAW, layout,
                             BUFFER_IN_NAME);

  input.transfer_to_gpu(data.particles);
  input.gl_bind_base(PARTICLE_SYSTEM_BINDING);

  Buffer<GLuint> output(BufferType::SSBO, BufferUsage::STATIC_DRAW, layout,
                        BUFFER_OUT_NAME);
  output.transfer_to_gpu(data.counter);
  output.gl_bind_base(ATOMIC_COUNTER_BINDING);

  MapTechnique::MapData map_data({
      // unary_op
      "0",
      // IOBufferData
      //   In
      "counters",
      "Counter_i",
      output.get_buffer_info(),
      //   Out
      "counters",
      "Counter_i",
      output.get_buffer_info(),
      // numVectors
      data.numGridPoints,
  });
  auto resetCounter = MapTechnique();
  resetCounter.init(std::move(map_data));

  CountingTechnique::CountingData counting_data({
      // IOBufferData
      // in
      "particles",
      "Particle_pos_mass",
      input.get_buffer_info(),
      // out
      "counters",
      "Counter_i",
      output.get_buffer_info(),
      // Uniforms
      data.gGridPos,
      data.gGridDim,
      data.gridSpacing,
      // Local Size
      // NumVectors
      data.numVectors,

  });
  auto binCount = CountingTechnique();
  binCount.init(std::move(counting_data));

  BenchmarkerCPU bench;
  bench.time("Total CPU time spent", [&binCount, numVectors = data.numVectors,
                                      &resetCounter,
                                      numGridPoints = data.numGridPoints]() {
    executeTest(10'000, [&binCount, &resetCounter, numVectors,
                         numGridPoints]() {
      BenchmarkerGPU::getInstance().time(
          "resetCounter", [&resetCounter, numGridPoints]() {
            resetCounter.dispatch_with_barrier(numGridPoints);
          });

      BenchmarkerGPU::getInstance().time("Counter", [&binCount, numVectors]() {
        binCount.dispatch_with_barrier(numVectors);
      });
    });
  });

  BenchmarkerGPU::getInstance().collect_times_last_frame();
  BenchmarkerGPU::getInstance().collect_times_last_frame();

  BenchmarkerGPU::write_to_file("Counting");
  bench.write_to_file("CountingCPU");
  auto m = output.transfer_to_cpu(std::size(data.counter));

  GLFWWindow::swapBuffers();

  return m;
}

