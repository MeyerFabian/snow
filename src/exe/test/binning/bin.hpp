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
#define BUFFER_OUT2_NAME "shader/buffers/particle_grid_offset.include.glsl"
#endif

#include <execution>
#include "../../../snow/shader/shader.hpp"
#include "../../../snow/utils/benchmarker.hpp"
#include "../../../test/binning/binTechnique.hpp"
#include "../../../test/map/mapTechnique.hpp"
#include "../../../test/test_util.hpp"

struct testData {
  GLuint numVectors;
  GLuint numGridPoints;
  std::vector<Particle_exp> particles;
  std::vector<GLuint> binning;
  std::vector<GLuint> grid_offset;
  glm::uvec3 gGridDim;
  PREC_VEC3_TYPE gGridPos;
  PREC_SCAL_TYPE gridSpacing;
};

struct OutputData {
  std::vector<GLuint> binning;
  std::vector<GLuint> grid_offset;
};

OutputData test(testData& data) {
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
  output.transfer_to_gpu(data.binning);
  output.gl_bind_base(ATOMIC_COUNTER_BINDING);
  MapTechnique::MapData map_data{
      // unary_op
      "0",
      // IOBufferData
      IOBufferData(
          {
              //   In
              "counters",
              "Counter_i",
              output.get_buffer_info(),
              data.numGridPoints,
          },
          {
              //   Out
              "counters",
              "Counter_i",
              output.get_buffer_info(),
              data.numGridPoints,
          }),
  };
  auto resetCounter = MapTechnique();
  resetCounter.init(std::move(map_data));

  BinningTechnique::BinningData binning_data{
      {
          data.gGridPos,
          data.gGridDim,
          data.gridSpacing,
      },
  };
#ifdef OUTPUT2

  Buffer<GLuint> output2(BufferType::SSBO, BufferUsage::STATIC_DRAW, layout,
                         BUFFER_OUT2_NAME);
  output2.transfer_to_gpu(data.grid_offset);
  output2.gl_bind_base(GRID_OFFSET_BINDING);

  // IOBufferData
  IO2BufferData io_data{
      {
          // in
          "particles",
          "Particle_pos_mass",
          input.get_buffer_info(),
          data.numVectors,
      },
      {
          // out
          "counters",
          "Counter_i",
          output.get_buffer_info(),
          data.numGridPoints,
      },
      {
          // out
          "gridOffsets",
          "GridOffset_i",
          output2.get_buffer_info(),
          data.numGridPoints,
      },
  };
#else
  // IOBufferData
  IOBufferData io_data(
      {
          // in
          "particles",
          "Particle_pos_mass",
          input.get_buffer_info(),
          data.numVectors,
      },
      {
          // out
          "counters",
          "Counter_i",
          output.get_buffer_info(),
          data.numGridPoints,
      });
#endif
  auto binCount = BinningTechnique();
  binCount.init(std::move(binning_data), std::move(io_data));

  BenchmarkerCPU bench;
  bench.time("Total CPU time spent", [&binCount, numVectors = data.numVectors,
                                      &resetCounter,
                                      numGridPoints = data.numGridPoints]() {
    executeTest(1, [&binCount, &resetCounter, numVectors, numGridPoints]() {
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
  auto m = output.transfer_to_cpu(std::size(data.binning));
  auto m2 = std::vector<GLuint>();
#ifdef OUTPUT2
  m2 = output2.transfer_to_cpu(std::size(data.grid_offset));
#endif
  GLFWWindow::swapBuffers();
  return {m, m2};
}

