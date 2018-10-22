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

#ifdef REORDER_SINGLE
#include "../../src/snow/particle/particle_indices.hpp"
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
#ifdef REORDER_SINGLE
  std::vector<ParticleIndices> particle_indices;
#else
  std::vector<GLuint> particle_indices;
#endif
  std::vector<Scan> scan;
  glm::uvec3 gGridDim;
  PREC_VEC3_TYPE gGridPos;
  PREC_SCAL_TYPE gridSpacing;
};
struct OutputData {
  std::vector<Particle_exp> particles;
#ifdef REORDER_SINGLE
  std::vector<ParticleIndices> particle_indices;
#else
  std::vector<GLuint> particle_indices;
#endif
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

  // particle_indices
#ifdef REORDER_SINGLE
  Buffer<ParticleIndices> particle_indices_buffer(
      BufferType::SSBO, BufferUsage::STATIC_DRAW, layout,
      "shader/buffers/particle_keys.include.glsl");
  particle_indices_buffer.transfer_to_gpu(data.particle_indices);
  particle_indices_buffer.gl_bind_base(PARTICLE_INDICES_BINDING);

#else
  Buffer<GLuint> particle_indices_buffer(
      BufferType::SSBO, BufferUsage::STATIC_DRAW, layout,
      "shader/buffers/particle_grid_offset.include.glsl");
  particle_indices_buffer.transfer_to_gpu(data.particle_indices);
  particle_indices_buffer.gl_bind_base(PARTICLE_INDICES_BINDING);

#endif
  // scan

  Buffer<Scan> scan_buffer(BufferType::SSBO, BufferUsage::STATIC_DRAW, layout,
                           "shader/buffers/scan_buffer.include.glsl");
  scan_buffer.transfer_to_gpu(data.scan);
  scan_buffer.gl_bind_base(SCAN_BUFFER_BINDING);

  /**********************************************************************
   *                    Techniques + IOData creation                    *
   **********************************************************************/
  BufferData counter_i = {
      "counters",
      "Counter_i",
      binning_buffer.get_buffer_info(),
      data.numGridPoints,
  };
  BufferData gridOffset_i = {
      "particle_indices",
      "GridOffset_i",
      particle_indices_buffer.get_buffer_info(),
      data.numParticles,
      0,
      1,
      "ParticleIndices_VAR_SIZE",
  };
  BufferData unsortedIndex_i = {
      "particle_indices",
      "UnsortedIndex_i",
      particle_indices_buffer.get_buffer_info(),
      data.numParticles,
      0,
      1,
      "ParticleIndices_VAR_SIZE",
  };
  // DOUBLE BUFFER
  BufferData Particle_pos_unsorted = {
      "particles",
      "Particle_pos_mass",
      particle_buffer.get_buffer_info(),
      data.numParticles,
      0,
      2,
      "Particle_exp_size",
  };
  BufferData Particle_pos_sorted = {
      "particles",
      "Particle_pos_mass",
      particle_buffer.get_buffer_info(),
      data.numParticles,
      1,  //!
      2,
      "Particle_exp_size",
  };
  BufferData Particle_2_unsorted = {
      "particles_2",         "", particle_buffer.get_buffer_info(),
      data.numParticles,     0,  2,

      "Particle_exp_2_size",
  };
  BufferData Particle_2_sorted = {
      "particles_2",
      "",
      particle_buffer.get_buffer_info(),
      data.numParticles,
      1,  //!
      2,
      "Particle_exp_2_size",
  };

  BufferData Scan_local_i = {
      "scans",
      "Scan_local_i",
      scan_buffer.get_buffer_info(),
      data.numGridPoints,
      0,
      1,
      "Scan_VAR_SIZE",
  };
  BufferData Scan_block_i = {
      // OUTPUT2
      "scans",
      "Scan_block_i",
      scan_buffer.get_buffer_info(),
      data.numGridPoints,
      0,
      1,
      "Scan_VAR_SIZE",
  };

  // map (reset counter)
  MapTechnique::MapData map_data{
      // unary_op
      "0",
      // IOBufferData
      IOBufferData(
          //   In
          {
              counter_i,  // INPUT
          },
          //   Out
          {
              counter_i,  // OUTPUT
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
      // in
      {
          Particle_pos_unsorted,  // INPUT
      },
      // out
      {
          counter_i,     // OUTPUT
          gridOffset_i,  // OUTPUT2
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
      // in
      {
          // INPUT
          counter_i,
      },
      // out
      {
          // OUTPUT
          Scan_local_i,
          Scan_block_i,
      },
  };

  auto scanPipeline = ScanPipeline();
  scanPipeline.init(std::move(scan_data), std::move(scan_io));

  // reorder
  ReorderTechnique::ReorderData reorder_data{
      // LocalSize local_size;
      {1024, 1, 1},
  // std::string filename;
#ifdef REORDER_SINGLE
#ifdef REORDER_KEYS
      "shader/compute/preprocess/reorder_keys.glsl",
#else  // REORDER_INDICES
      "shader/compute/preprocess/reorder_indices.glsl",
#endif
#else
      "shader/compute/preprocess/reorder.glsl",
#endif
      // GLuint scan_block_size;
      scanPipeline.get_scan_block_size(),
      data.gGridPos,
      data.gGridDim,
      data.gridSpacing,
  };

  IOBufferData reorder_io{
      // in
      {
          // INPUT offsets
          gridOffset_i,
          // INPUT2 scan_local
          Scan_local_i,
          // INPUT3 scan_block
          Scan_block_i,
          // INPUT4 container to sort
          Particle_pos_unsorted,
          Particle_2_unsorted,

      },
      // out
      {
  // OUTPUT container to put sort
#ifdef REORDER_SINGLE
          unsortedIndex_i,
#else
          Particle_pos_sorted,
          Particle_2_sorted,
#endif
      },
  };

  auto reordering = ReorderTechnique();
  reordering.init(std::move(reorder_data), std::move(reorder_io));

  /**********************************************************************
   *                         execute dispatches                         *
   **********************************************************************/

  BenchmarkerCPU bench;
  bench.time("Total CPU time spent", [&resetCounter,  // resetcnt
                                      &binCount,      // bin
                                      &scanPipeline,  // scan
                                      &reordering,    // reorder
                                      numParticles = data.numParticles,
                                      numGridPoints = data.numGridPoints]() {
    executeTest(10'000, [&resetCounter,  // reset
                         &binCount,      // bin
                         &scanPipeline,  // scan
                         &reordering,    // reorder
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
      BenchmarkerGPU::getInstance().time(
          "Reorder Particles", [&reordering, numParticles]() {
            reordering.dispatch_with_barrier({numParticles});
          });
    });
  });

  BenchmarkerGPU::getInstance().collect_times_last_frame();
  BenchmarkerGPU::getInstance().collect_times_last_frame();
  BenchmarkerGPU::write_to_file("Reordering");
  bench.write_to_file("ReorderingCPU");

  // get buffer from gpu
  auto par1 = particle_buffer.transfer_to_cpu(std::size(data.particles));
  auto particle_index =
      particle_indices_buffer.transfer_to_cpu(std::size(data.particle_indices));

  GLFWWindow::swapBuffers();
  return {par1, particle_index};
}

