#ifndef GLM_INCLUDE_GUARD
#define GLM_INCLUDE_GUARD
#include "../../../snow/utils/defines.hpp"

#include <glm/gtc/random.hpp>
#include <numeric>
#include "../../../../shader/shared_hpp/buffer_bindings.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>
#include "../../../snow/shader/shader.hpp"
#endif

#include "../../src/snow/buffer/c_scan.hpp"
#include "../../src/snow/particle/particle_exp.hpp"

#include <execution>
#include "../../../snow/utils/benchmarker.hpp"
#include "../../../test/binning/binTechnique.hpp"
#include "../../../test/map/mapTechnique.hpp"
#include "../../../test/reorder/reorderTechnique.hpp"
#include "../../../test/scan/ScanPipeline.hpp"
#include "../../../test/test_util.hpp"

struct testData {
  GLuint numParticles;
  GLuint numGridPoints;
  std::vector<Particle_exp> particles;
  std::vector<Particle_exp_2> particles2;
  std::vector<GLuint> binning;
  std::vector<GLuint> grid_offset;
  std::vector<Scan> scan;
  glm::uvec3 gGridDim;
  PREC_VEC3_TYPE gGridPos;
  PREC_SCAL_TYPE gridSpacing;
};
struct OutputData {
  std::vector<Particle_exp> particles;
};

OutputData test(testData& data) {
  GLFWWindow();
#ifdef AOS_LAYOUT
  BufferLayout layout = BufferLayout::AOS;
#else
  BufferLayout layout = BufferLayout::SOA;
#endif

  /**********************************************************************
   *                          Buffer creations                          *
   **********************************************************************/

  // Particle_exp
  Buffer<Particle_exp> particle_buffer(
      BufferType::SSBO, BufferUsage::STATIC_DRAW, layout,
      "shader/buffers/particle_system.include.glsl");

  particle_buffer.transfer_to_gpu(data.particles);
  particle_buffer.gl_bind_base(PARTICLE_SYSTEM_BINDING);
  // Particle_exp2
  Buffer<Particle_exp_2> particle_2_buffer(
      BufferType::SSBO, BufferUsage::STATIC_DRAW, layout,
      "shader/buffers/particle_system.include.glsl");

  particle_2_buffer.transfer_to_gpu(data.particles2);
  particle_2_buffer.gl_bind_base(PARTICLE_SYSTEM_BINDING_2);

  // binning
  Buffer<GLuint> binning_buffer(BufferType::SSBO, BufferUsage::STATIC_DRAW,
                                layout,
                                "shader/buffers/atomic_counter.include.glsl");
  binning_buffer.transfer_to_gpu(data.binning);
  binning_buffer.gl_bind_base(ATOMIC_COUNTER_BINDING);

  // grid_offset
  Buffer<GLuint> grid_offset_buffer(
      BufferType::SSBO, BufferUsage::STATIC_DRAW, layout,
      "shader/buffers/particle_grid_offset.include.glsl");
  grid_offset_buffer.transfer_to_gpu(data.grid_offset);
  grid_offset_buffer.gl_bind_base(GRID_OFFSET_BINDING);

  // scan

  Buffer<Scan> scan_buffer(BufferType::SSBO, BufferUsage::STATIC_DRAW, layout,
                           "shader/buffers/scan_buffer.include.glsl");
  scan_buffer.transfer_to_gpu(data.scan);
  scan_buffer.gl_bind_base(SCAN_BUFFER_BINDING);

  /**********************************************************************
   *                    Techniques + IOData creation                    *
   **********************************************************************/

  // map (reset counter)
  MapTechnique::MapData map_data{
      // unary_op
      "0",
      // IOBufferData
      IOBufferData(
          {

              //   In
              {
                  // INPUT
                  "counters",
                  "Counter_i",
                  binning_buffer.get_buffer_info(),
                  data.numGridPoints,
              },
          },
          {
              //   Out
              {
                  // OUTPUT
                  "counters",
                  "Counter_i",
                  binning_buffer.get_buffer_info(),
                  data.numGridPoints,
              },
          }),
  };
  auto resetCounter = MapTechnique();
  resetCounter.init(std::move(map_data));
  // bin
  BinningTechnique::BinningData binning_data{
      {
          data.gGridPos,
          data.gGridDim,
          data.gridSpacing,
      },
  };
  IOBufferData map_io{
      {
          // in
          {
              // INPUT
              "particles",
              "Particle_pos_mass",
              particle_buffer.get_buffer_info(),
              data.numParticles,
          },
      },
      {
          // out
          {

              // OUTPUT
              "counters",
              "Counter_i",
              binning_buffer.get_buffer_info(),
              data.numGridPoints,
          },
          {

              // OUTPUT2
              "gridOffsets",
              "GridOffset_i",
              grid_offset_buffer.get_buffer_info(),
              data.numParticles,
          },
      },
  };
  auto binCount = BinningTechnique();
  binCount.init(std::move(binning_data), std::move(map_io));

  // scan

  ScanTechnique::ScanData scan_data{
      // local_size
      {1024, 1, 1},
      // shader
      "shader/compute/preprocess/scan.glsl",
      // unary_op_return_type
      "uint",
      // unary_op
      "value",
      // gl_binary_op_neutral_elem
      "0",
      // gl_biunary_op
      "left+right",
      data.numGridPoints,
      // raking
      2,
  };

  IOBufferData scan_io{
      {
          // in
          {
              // INPUT
              "counters",
              "Counter_i",
              binning_buffer.get_buffer_info(),
              data.numGridPoints,
          },
      },
      {

          // out
          {
              // OUTPUT
              "scans",
              "Scan_local_i",
              scan_buffer.get_buffer_info(),
              data.numGridPoints,
          },
          {
              // OUTPUT2
              "scans",
              "Scan_block_i",
              scan_buffer.get_buffer_info(),
              data.numGridPoints,
          },
      },
  };

  auto scanPipeline = ScanPipeline();
  scanPipeline.init(std::move(scan_data), std::move(scan_io));

  // reorder
  ReorderTechnique::ReorderData{
      // LocalSize local_size;
      {1024, 1, 1},
      // std::string filename;
      "shader/compute/preprocess/reorder.glsl",
      // GLuint scan_block_size;
      scanPipeline.get_scan_block_size(),
  };

  IOBufferData reorder_io{
      {
          // in
          {
              // INPUT offsets
          },
          {
              // INPUT2 scan_local
          },
          {
              // INPUT3 scan_block
          },
          {
              // INPUT3+X container to sort
          },
      },
      {
          // out
          {
              // OUTPUTX container to put sort
          },
      },
  };

  // execute test / dispatches
  BenchmarkerCPU bench;
  bench.time("Total CPU time spent", [&resetCounter,  // reset
                                      &binCount,      // bin
                                      &scanPipeline,  // scan
                                      // reorder
                                      numParticles = data.numParticles,
                                      numGridPoints = data.numGridPoints]() {
    executeTest(1, [&resetCounter,  // reset
                    &binCount,      // bin
                    &scanPipeline,  // scan
                                    // reorder
                    numParticles, numGridPoints]() {
      // reset
      BenchmarkerGPU::getInstance().time(
          "resetCounter", [&resetCounter, numGridPoints]() {
            resetCounter.dispatch_with_barrier(numGridPoints);
          });

      // bin
      BenchmarkerGPU::getInstance().time(
          "Counter", [&binCount, numParticles]() {
            binCount.dispatch_with_barrier(numParticles);
          });
      // scan
      scanPipeline.run(numGridPoints);
      // reorder
      // uniform for reorder
    });
  });

  BenchmarkerGPU::getInstance().collect_times_last_frame();
  BenchmarkerGPU::getInstance().collect_times_last_frame();
  BenchmarkerGPU::write_to_file("Reordering");
  bench.write_to_file("ReorderingCPU");

  // get buffer from gpu
  auto par1 = particle_buffer.transfer_to_cpu(std::size(data.particles));

  GLFWWindow::swapBuffers();
  return {par1};
}

