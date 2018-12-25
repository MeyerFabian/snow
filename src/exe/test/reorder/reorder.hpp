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
#include "../../../test/BufferData.hpp"
#include "../../../test/binning/binTechnique.hpp"
#include "../../../test/map/mapTechnique.hpp"
#include "../../../test/reorder/reorderTechnique.hpp"
#include "../../../test/scan/ScanPipeline.hpp"
#include "../../../test/test_util.hpp"
#include "../../src/snow/grid/grid_def.hpp"
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
  GridDefines grid_def;
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

  Buffer<GridDefines> grid_def_buffer(
      BufferType::UNIFORM, BufferUsage::DYNAMIC_DRAW, BufferLayout::AOS);

  grid_def_buffer.transfer_to_gpu(std::vector<GridDefines>{data.grid_def});
  grid_def_buffer.gl_bind_base(GRID_DEFINES_BINDING);

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
      "shader/buffers/particle_indices.include.glsl");
  particle_indices_buffer.transfer_to_gpu(data.particle_indices);
  particle_indices_buffer.gl_bind_base(PARTICLE_INDICES_BINDING);

#else
  Buffer<GLuint> particle_indices_buffer(
      BufferType::SSBO, BufferUsage::STATIC_DRAW, layout,
      "shader/buffers/particle_grid_offset.include.glsl");
  particle_indices_buffer.transfer_to_gpu(data.particle_indices);
  particle_indices_buffer.gl_bind_base(PARTICLE_GRIDOFFSET_BINDING);

#endif
  // scan

  Buffer<Scan> scan_buffer(BufferType::SSBO, BufferUsage::STATIC_DRAW, layout,
                           "shader/buffers/scan_buffer.include.glsl");
  scan_buffer.transfer_to_gpu(data.scan);
  scan_buffer.gl_bind_base(SCAN_BUFFER_BINDING);

  /**********************************************************************
   *                    Techniques + IOData creation                    *
   **********************************************************************/
  auto counter_i =
      BufferData("counters", "Counter_i", binning_buffer.get_buffer_info(),
                 data.numGridPoints);
#ifdef REORDER_SINGLE
  auto gridOffset_i =
      BufferData("particle_indices", "GridOffset_i",
                 particle_indices_buffer.get_buffer_info(), data.numParticles,
                 1, "0", "ParticleIndices_VAR_SIZE");
#else
  auto gridOffset_i = BufferData(
      "gridOffsets", "GridOffset_i", particle_indices_buffer.get_buffer_info(),
      data.numParticles, 1, "0", "Particle_GridOffset_VAR_SIZE");
#endif

  auto unsortedIndex_i =
      BufferData("particle_indices", "UnsortedIndex_i",
                 particle_indices_buffer.get_buffer_info(), data.numParticles,
                 1, "0", "ParticleIndices_VAR_SIZE");
  auto Particle_pos_unsorted = BufferData(
      "particles", "Particle_pos_vol", particle_buffer.get_buffer_info(),
      data.numParticles, 2, "0", "Particle_exp_size");
  auto Particle_pos_sorted = BufferData(
      "particles", "Particle_pos_vol", particle_buffer.get_buffer_info(),
      data.numParticles, 2, "1", "Particle_exp_size");
  auto Particle_2_unsorted =
      BufferData("particles_2", "", particle_buffer.get_buffer_info(),
                 data.numParticles, 2, "0",

                 "Particle_exp_2_size");
  auto Particle_2_sorted =
      BufferData("particles_2", "", particle_buffer.get_buffer_info(),
                 data.numParticles, 2, "1", "Particle_exp_2_size");

  auto Scan_local_i =
      BufferData("scans", "Scan_local_i", scan_buffer.get_buffer_info(),
                 data.numGridPoints, 1, "0", "Scan_VAR_SIZE");
  auto Scan_block_i =
      BufferData("scans", "Scan_block_i", scan_buffer.get_buffer_info(),
                 data.numGridPoints, 1, "0", "Scan_VAR_SIZE");
  // map (reset counter)
  MapTechnique::MapData map_data{
      "shader/compute/mapreduce/map.glsl",
      // unary_op
      "0",
  };

  IOBufferData io_map;
  // INPUT
  io_map.in_buffer.push_back(std::make_unique<BufferData>(counter_i));
  // OUTPUT
  io_map.out_buffer.push_back(std::make_unique<BufferData>(counter_i));

  auto resetCounter = MapTechnique();
  resetCounter.init(std::move(map_data), std::move(io_map));
  // bin
  BinningTechnique::BinningData binning_data{
      "shader/compute/preprocess/bin.glsl",

  };
  IOBufferData io_bin;
  // INPUT
  io_bin.in_buffer.push_back(
      std::make_unique<BufferData>(Particle_pos_unsorted));

  // OUTPUT
  io_bin.out_buffer.push_back(std::make_unique<BufferData>(counter_i));

  // OUTPUT2
  io_bin.out_buffer.push_back(std::make_unique<BufferData>(gridOffset_i));

  auto binCount = BinningTechnique();
  binCount.init(std::move(binning_data), std::move(io_bin));

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

  IOBufferData io_scan;

  // INPUT
  io_scan.in_buffer.push_back(std::make_unique<BufferData>(counter_i));

  // OUTPUT
  io_scan.out_buffer.push_back(std::make_unique<BufferData>(Scan_local_i));

  // OUTPUT2
  io_scan.out_buffer.push_back(std::make_unique<BufferData>(Scan_block_i));

  auto scanPipeline = ScanPipeline();
#ifdef SCAN_DIRECT_WRITE_BACK
  scanPipeline.initDirectWriteBack(std::move(scan_data), std::move(io_scan));
#else
  scanPipeline.init(std::move(scan_data), std::move(io_scan));
#endif
  // reorder
  ReorderTechnique::ReorderData reorder_data{
      // LocalSize local_size;
      {32, 1, 1},
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
#ifdef SCAN_DIRECT_WRITE_BACK
      true,
#else
      false,
      scanPipeline.get_scan_block_size(),
#endif
  };

  IOBufferData io_reorder;

  // INPUT
  io_reorder.in_buffer.push_back(std::make_unique<BufferData>(gridOffset_i));
  // INPUT2
  io_reorder.in_buffer.push_back(std::make_unique<BufferData>(Scan_local_i));
  // INPUT3
  io_reorder.in_buffer.push_back(std::make_unique<BufferData>(Scan_block_i));
  // INPUT4
  io_reorder.in_buffer.push_back(
      std::make_unique<BufferData>(Particle_pos_unsorted));
  // INPUT5
  io_reorder.in_buffer.push_back(
      std::make_unique<BufferData>(Particle_2_unsorted));

#ifdef REORDER_SINGLE
  // OUTPUT
  io_reorder.out_buffer.push_back(
      std::make_unique<BufferData>(unsortedIndex_i));
#else
  // OUTPUT
  io_reorder.out_buffer.push_back(
      std::make_unique<BufferData>(Particle_pos_sorted));
  // OUTPUT2
  io_reorder.out_buffer.push_back(
      std::make_unique<BufferData>(Particle_2_sorted));
#endif
  auto reordering = ReorderTechnique();
  reordering.init(std::move(reorder_data), std::move(io_reorder));

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
            resetCounter.dispatch_with_barrier({numGridPoints, true, 2});
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

