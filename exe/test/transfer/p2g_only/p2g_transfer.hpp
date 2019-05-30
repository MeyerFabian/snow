#ifndef P2G_TRANSFER_HPP
#define P2G_TRANSFER_HPP
#include <glm/gtc/random.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include "../../../../src/core/buffer/BufferData/BufferData.hpp"
#include "../../../../src/core/object/grid/grid_def.hpp"
#include "../../../../src/core/object/grid/gridpoint.hpp"
#include "../../../../src/core/object/particle/particle_exp.hpp"
#include "../../../../src/core/simulation/computingTechnique/block/BlockPipeline.hpp"
#include "../../../../src/core/simulation/computingTechnique/p2g/p2g_atomic_global.hpp"
#include "../../../../src/core/simulation/computingTechnique/p2g/p2g_shared.hpp"
#include "../../../../src/core/simulation/computingTechnique/reorder/countingSortPipeline.hpp"

#include "../../../../src/test/test_util.hpp"
#include "../../../../src/test/transfer/testData.hpp"

#include "../../../../src/test/OutputBufferData.hpp"
#include "../../../../src/test/block/testBlockPipeline.hpp"
#include "../../../../src/test/reorder/testCountSort.hpp"
#include "../../../../src/test/transfer/testBuffers.hpp"
#include "../../../../src/test/transfer/testP2G.hpp"

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
#endif /* end of include guard: P2G_TRANSFER_HPP */

