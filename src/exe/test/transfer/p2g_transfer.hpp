#ifndef ATOMIC_HPP_CN2ZL6DH
#define ATOMIC_HPP_CN2ZL6DH
#include <glm/gtc/random.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include "../../../test/BufferData.hpp"
#include "../../../test/block/BlockPipeline.hpp"
#include "../../../test/p2g/p2g_atomic_global.hpp"
#include "../../../test/p2g/p2g_shared.hpp"
#include "../../../test/reorder/countingSortPipeline.hpp"
#include "../../../test/test_util.hpp"
#include "../../src/snow/grid/grid_def.hpp"
#include "../../src/snow/grid/gridpoint.hpp"
#include "../../src/snow/particle/particle_exp.hpp"
#include "testData.hpp"

#include "../../../test/OutputBufferData.hpp"

#include "../../../test/buffers/testBuffers.hpp"

#include "../../../test/reorder/testCountSort.hpp"

#include "../../../test/block/testBlockPipeline.hpp"

#include "../../../test/p2g/testP2G.hpp"
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

                 &tp2g, &numParticles, &numGridPoints]() {
               executeTest(1, [
#ifdef FULL_SORTED
                                  &ts,
#endif
#ifdef BLOCK_COMPACTION
                                  &tp,
#endif

                                  &tp2g, &numParticles, &numGridPoints]() {
#ifdef FULL_SORTED
                 ts.run(numGridPoints, numParticles);
#endif
#ifdef BLOCK_COMPACTION
                 tp.run(numGridPoints);
#endif
                 tp2g.run(numGridPoints, numParticles);
               });
             });

  BenchmarkerGPU::getInstance().collect_times_last_frame();
  BenchmarkerGPU::getInstance().collect_times_last_frame();

  BenchmarkerGPU::write_to_file("p2gTransfer");
  bench.write_to_file("p2gTransferCPU");
  return {buffers.getGridPoints(numGridPoints)};
}
#endif /* end of include guard: COUNT_SRT_HPP_CN2ZL6DH */

