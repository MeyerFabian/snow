#ifndef ATOMIC_HPP_CN2ZL6DH
#define ATOMIC_HPP_CN2ZL6DH
#include <glm/gtc/random.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include "../../../../shader/shared_hpp/buffer_bindings.hpp"
#include "../../../test/BufferData.hpp"
#include "../../../test/block/BlockPipeline.hpp"
#include "../../../test/map/mapTechnique.hpp"
#include "../../../test/p2g/p2g_atomic_global.hpp"
#include "../../../test/p2g/p2g_shared.hpp"
#include "../../../test/reorder/countingSortPipeline.hpp"
#include "../../../test/test_util.hpp"
#include "../../src/snow/grid/grid_def.hpp"
#include "../../src/snow/grid/gridpoint.hpp"
#include "../../src/snow/particle/particle_exp.hpp"
#include "testData.hpp"
struct OutputData {
  std::vector<Gridpoint> grid;
};

OutputData test(testData data) {
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

  // Gridpoints
  Buffer<Gridpoint> grid_buffer(BufferType::SSBO, BufferUsage::STATIC_DRAW,
                                layout,
                                "shader/buffers/uniform_grid.include.glsl");
  grid_buffer.resize_buffer(data.numGridPoints);
  grid_buffer.gl_bind_base(UNIFORM_GRID_BINDING);

  /**********************************************************************
   *                    Techniques + IOData creation                    *
   **********************************************************************/
  auto gridpoint_vel_mass = BufferData(
      "gridpoints", "Gridpoint_vel_mass", grid_buffer.get_buffer_info(),
      data.numGridPoints, 1, "0", "Gridpoint_size");

#ifdef FULL_SORTED
  // full sort double buffered
  auto Particle_pos_unsorted = BufferData(
      "particles", "Particle_pos_vol", particle_buffer.get_buffer_info(),
      data.numParticles, 2, "0", "Particle_exp_size");

  auto Particle_2_unsorted =
      BufferData("particles_2", "", particle_buffer.get_buffer_info(),
                 data.numParticles, 2, "0", "Particle_exp_2_size");
#else
  auto Particle_pos_unsorted = BufferData(
      "particles", "Particle_pos_vol", particle_buffer.get_buffer_info(),
      data.numParticles, 1, "0", "Particle_exp_size");

  auto Particle_2_unsorted =
      BufferData("particles_2", "", particle_buffer.get_buffer_info(),
                 data.numParticles, 1, "0", "Particle_exp_2_size");
#endif

#ifdef SORTED
  CountingSortPipeline::CountingSortData cnt_srt_data{
      layout,
      data.numGridPoints,
  };

  CountingSortPipeline cnt_srt_pipeline;
  IOBufferData io_cnt_srt;
  // OUTPUT
  io_cnt_srt.out_buffer.push_back(
      std::make_unique<BufferData>(Particle_pos_unsorted));
  io_cnt_srt.out_buffer.push_back(
      std::make_unique<BufferData>(Particle_2_unsorted));
#endif

#if defined(FULL_SORTED)
  cnt_srt_pipeline.initFullSort(std::move(cnt_srt_data), std::move(io_cnt_srt));
#elif defined(INDEX_SORTED)
#if defined(INDEX_WRITE_SORTED)
  cnt_srt_pipeline.initIndexWriteSort(std::move(cnt_srt_data),
                                      std::move(io_cnt_srt));
#else

  cnt_srt_pipeline.initIndexReadSort(std::move(cnt_srt_data),
                                     std::move(io_cnt_srt));
#endif
#endif

#ifdef BLOCK_COMPACTION
  std::shared_ptr<BlockPipeline> block_pipeline =
      std::make_shared<BlockPipeline>();

  BlockPipeline::BlockData block_data{
      data.numGridPoints,
      layout,
  };

  IOBufferData io_block;
  // in
  io_block.in_buffer.push_back(cnt_srt_pipeline.getGridCounter());
  io_block.out_buffer.push_back(
      std::make_unique<BufferData>(gridpoint_vel_mass));
#ifdef SHARED_PULL
  block_pipeline->initHalo(std::move(block_data), std::move(io_block));
#else
  block_pipeline->initBlock(std::move(block_data), std::move(io_block));
#endif
#endif
  auto resetGridVel = MapTechnique();
  MapTechnique::MapData map_data{
      "shader/compute/mapreduce/mapSingle.glsl",
      // unary_op
      "vec4(0.0,0.0,0.0,0.0)",
  };

  IOBufferData io_map;
  // INPUT
  io_map.in_buffer.push_back(std::make_unique<BufferData>(gridpoint_vel_mass));
  // OUTPUT
  io_map.out_buffer.push_back(std::make_unique<BufferData>(gridpoint_vel_mass));

  resetGridVel.init(std::move(map_data), std::move(io_map));

  /**********************************************************************
   *                               p2g_io in                            *
   **********************************************************************/

  IOBufferData p2g_io;
#ifdef FULL_SORTED
#ifdef SHARED
  auto particles_sorted = cnt_srt_pipeline.getSortedBufferDataAccess();
#else   // NOT SHARED
  auto particles_sorted = cnt_srt_pipeline.getSortedBufferData();
#endif  // SHARED
  for (auto& particle_sorted : particles_sorted) {
    p2g_io.in_buffer.push_back(particle_sorted->cloneBufferDataInterface());
  }
#else   // NOT FULL_SORTED
  p2g_io.in_buffer.push_back(
      std::make_unique<BufferData>(Particle_pos_unsorted));
#endif  // FULL_SORTED

  /**********************************************************************
   *                               p2g_io out                           *
   **********************************************************************/

#ifdef BLOCK_COMPACTION
#if defined(PUSH_SYNC) || defined(SHARED_PULL)
  auto grid_block_compact = block_pipeline->getBlockBufferDataAccess();
#else
  auto grid_block_compact = block_pipeline->getBlockBufferData();
#endif
  for (auto& block_buffer_data : grid_block_compact) {
    p2g_io.out_buffer.push_back(block_buffer_data->cloneBufferDataInterface());
  }
#else
  p2g_io.out_buffer.push_back(std::make_unique<BufferData>(gridpoint_vel_mass));
#endif

  /**********************************************************************
   *                               p2g technique                        *
   **********************************************************************/

#if defined(SHARED)  // 1

  auto p2gTransfer = P2G_shared();

#if defined(SHARED_ATOMIC_LOOP_REV)  // 2

  P2G_shared::P2GData p2g_data{
      data.grid_def.gGridDim,
  };
  p2gTransfer.init_atomic_loop_reverse(std::move(p2g_data), std::move(p2g_io));
#elif defined(SHARED_ATOMIC)  // 2

#if defined(SHARED_BATCHING_MULT_PART)  // 3
  P2G_shared::P2GBatchingData p2g_data{
      data.grid_def.gGridDim,
#ifdef BLOCK_COMPACTION                 // 4
      block_pipeline,
#else                                   // NOT BLOCK_COMPACT
      std::nullopt,
#endif                                  // BLOCK_COMPACT 4
      SHARED_BATCHING_MULT_PART,
  };
  p2gTransfer.init_atomic_batching(std::move(p2g_data), std::move(p2g_io));
#else                                   // NO SHARED_BATCHING
  P2G_shared::P2GData p2g_data{
      data.grid_def.gGridDim,
#ifdef BLOCK_COMPACTION                 // 4
      block_pipeline,
#endif                                  // BLOCK_COMPACTION 4
  };
  p2gTransfer.init_atomic(std::move(p2g_data), std::move(p2g_io));
#endif                                  // SHARED_BATCHING 3

#elif defined(PUSH_SYNC)  // 2

#if defined(SHARED_BATCHING_MULT_PART)  // 3
  P2G_shared::P2GBatchingData p2g_data{
      data.grid_def.gGridDim,
      block_pipeline,
      SHARED_BATCHING_MULT_PART,
  };
  p2gTransfer.init_sync_batching(std::move(p2g_data), std::move(p2g_io));
#else                                   // NO_SHARED_BATCHING
  P2G_shared::P2GData p2g_data{
      data.grid_def.gGridDim,
      block_pipeline,
  };
  p2gTransfer.init_sync(std::move(p2g_data), std::move(p2g_io));

#endif  // SHARED_BATCHING 3

#elif defined(SHARED_PULL)  // 1

  P2G_shared::P2GData p2g_data{
      data.grid_def.gGridDim,
      block_pipeline,
  };
  p2gTransfer.init_pull_simple(std::move(p2g_data), std::move(p2g_io));
#endif                      // SHARED
#elif defined(ATOMIC_LOOP)  // 1
  auto p2gTransfer = P2G_atomic_global();
  P2G_atomic_global::P2GData p2g_data{};

  p2gTransfer.init_looping(std::move(p2g_data), std::move(p2g_io));
#else                       // 1
  // global w/o looping
  auto p2gTransfer = P2G_atomic_global();
  P2G_atomic_global::P2GData p2g_data{};
  p2gTransfer.init_too_parallel(std::move(p2g_data), std::move(p2g_io));
#endif

  /**********************************************************************
   *                         execute dispatches                         *
   **********************************************************************/

  BenchmarkerCPU bench;
  bench.time("Total CPU time spent",
             [
#ifdef FULL_SORTED
                 &cnt_srt_pipeline,
#endif
#ifdef BLOCK_COMPACTION
                 &block_pipeline,
#endif
                 &resetGridVel, &p2gTransfer, numParticles = data.numParticles,
                 numGridPoints = data.numGridPoints]() {
               executeTest(1, [
#ifdef FULL_SORTED
                                  &cnt_srt_pipeline,
#endif
#ifdef BLOCK_COMPACTION
                                  &block_pipeline,
#endif
                                  &resetGridVel, &p2gTransfer, &numParticles,
                                  &numGridPoints]() {
#ifdef FULL_SORTED
                 cnt_srt_pipeline.run({numParticles, numGridPoints});
#endif
#ifdef BLOCK_COMPACTION

                 block_pipeline->run({numGridPoints});
#endif
                 BenchmarkerGPU::getInstance().time(
                     "resetGridVel", [&resetGridVel, &numGridPoints]() {
                       resetGridVel.dispatch_with_barrier({numGridPoints});
                     });
#if defined(SHARED)
                 BenchmarkerGPU::getInstance().time(
                     "p2gTransfer_shared", [&p2gTransfer, &numParticles]() {
                       p2gTransfer.dispatch_with_barrier({});
                     });

#else

                                  BenchmarkerGPU::getInstance().time(
                                      "p2gTransfer_atomic", [&p2gTransfer,
                 &numParticles]() {
                                        p2gTransfer.dispatch_with_barrier({numParticles});
                                      });
#endif
               });
             });

  BenchmarkerGPU::getInstance().collect_times_last_frame();
  BenchmarkerGPU::getInstance().collect_times_last_frame();

  BenchmarkerGPU::write_to_file("p2gTransfer");
  bench.write_to_file("p2gTransferCPU");
  return {
      grid_buffer.transfer_to_cpu(data.numGridPoints),
  };
}
#endif /* end of include guard: COUNT_SRT_HPP_CN2ZL6DH */

