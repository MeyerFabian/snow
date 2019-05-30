#ifndef ATOMIC_HPP_CN2ZL6DH
#define ATOMIC_HPP_CN2ZL6DH
#include <glm/gtc/random.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include "../../../../src/core/buffer/BufferData/BufferData.hpp"
#include "../../../../src/core/object/grid/grid_def.hpp"
#include "../../../../src/core/object/grid/gridpoint_exp.hpp"
#include "../../../../src/core/object/particle/particle_exp.hpp"
#include "../../../../src/core/simulation/computingTechnique/block/BlockPipeline.hpp"
#include "../../../../src/core/simulation/computingTechnique/map/mapTechnique.hpp"
#include "../../../../src/core/simulation/computingTechnique/p2g/p2g_atomic_global.hpp"
#include "../../../../src/core/simulation/computingTechnique/p2g/p2g_shared.hpp"
#include "../../../../src/core/simulation/computingTechnique/reorder/countingSortPipeline.hpp"
#include "../../../../src/test/test_util.hpp"
#include "../../../../src/test/transfer/testData.hpp"

#include "../../../../src/test/OutputBufferData.hpp"

#include "../../../../src/test/transfer/testBuffers.hpp"

#include "../../../../src/test/reorder/testCountSort.hpp"

#include "../../../../src/test/block/testBlockPipeline.hpp"

#include "../../../../src/test/transfer/testP2G.hpp"

#include "../../../../src/test/transfer/testG2P.hpp"
struct OutputData {
  std::vector<Gridpoint> grid;
  std::vector<Particle_exp> particles;
};

OutputData test(testData data) {
  GLFWWindow();

#ifdef AOS_LAYOUT
  BufferLayout layout = BufferLayout::AOS;
#else
  BufferLayout layout = BufferLayout::SOA;
#endif

  auto numGridPoints = data.numGridPoints;
  auto numParticles = data.numParticles;
  auto gGridDim = data.grid_def.gGridDim;

  auto buffers = TestBuffers(std::move(data), layout);
  auto buffersData = buffers.getBufferData();
#ifdef FULL_SORTED
  TestCountSort::TestCountSortData ts_data{
      numGridPoints,
      layout,
  };

  TestCountSort ts(std::move(ts_data), buffersData);
#endif
#ifdef BLOCK_COMPACTION
  TestBlockPipeline::TestBlockPipelineData tp_data{
      numGridPoints,
      layout,
      ts.getGridCounter(),
  };
  TestBlockPipeline tp(std::move(tp_data), buffersData);
#endif

  TestP2G::TestP2GData tp2g_data{
      gGridDim,
#ifdef FULL_SORTED
      ts.getSortedBufferData(),
      ts.getSortedBufferDataAccess(),
#endif
#ifdef BLOCK_COMPACTION
      tp.getBlockBufferData(),
      tp.getBlockBufferDataAccess(),
      tp.getIndirectDispatch(),
#endif
  };
  TestP2G tp2g(std::move(tp2g_data), buffersData);

  MapTechnique mass_divide = MapTechnique();
  MapTechnique::MapData map_data{
      "shader/compute/map/map.glsl",
      // unary_op
      "(value.w>1e-8f)?vec4(value.xyz/"
      "value.w,value.w):vec4(0.0f,0.0f,0.0f,value.w)",
  };

  IOBufferData io_map;
  // INPUT
  io_map.in_buffer.push_back(
      std::make_unique<BufferData>(buffersData.gridpoint_vel_mass));
  // OUTPUT
  io_map.out_buffer.push_back(
      std::make_unique<BufferData>(buffersData.gridpoint_vel_mass));

  mass_divide.init(std::move(map_data), std::move(io_map));

  TestG2P::TestG2PData tg2p_data{
      gGridDim,
#ifdef FULL_SORTED
      ts.getSortedBufferData(),
      ts.getSortedBufferDataAccess(),
#endif
#ifdef BLOCK_COMPACTION
      tp.getBlockBufferData(),
      tp.getIndirectDispatch(),
#endif
  };
  TestG2P tg2p(std::move(tg2p_data), buffersData);

  /**********************************************************************
   *                         execute dispatches                         *
   **********************************************************************/
  BenchmarkerCPU bench;
  bench.time("Total CPU time spent",
             [
#ifdef FULL_SORTED
                 &ts,
#endif
#ifdef BLOCK_COMPACTION
                 &tp,
#endif
                 &tp2g, &tg2p, &mass_divide, &numParticles, &numGridPoints]() {
               executeTest(1000, [
#ifdef FULL_SORTED
                                     &ts,
#endif
#ifdef BLOCK_COMPACTION
                                     &tp,
#endif

                                     &tp2g, &tg2p, &mass_divide, &numParticles,
                                     &numGridPoints]() {
#ifdef FULL_SORTED
                 ts.run(numGridPoints, numParticles);
#endif
#ifdef BLOCK_COMPACTION
                 tp.run(numGridPoints);
#endif
                 tp2g.run(numGridPoints, numParticles);

                 BenchmarkerGPU::getInstance().time(
                     "massDivide", [&mass_divide, &numGridPoints]() {
                       mass_divide.dispatch_with_barrier({numGridPoints});
                     });

                 tg2p.run(numGridPoints, numParticles);
               });
             });

  BenchmarkerGPU::getInstance().collect_times_last_frame();
  BenchmarkerGPU::getInstance().collect_times_last_frame();

  BenchmarkerGPU::write_to_file("roundTripTransfer");
  bench.write_to_file("roundTripCPU");
  return {buffers.getGridPoints(numGridPoints),
          buffers.getParticles(numParticles)};
}
#endif /* end of include guard: COUNT_SRT_HPP_CN2ZL6DH */

