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

#include "../../../test/g2p/testG2P.hpp"
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
  TestCountSort::TestCountSortData ts_data{
      numGridPoints,
      layout,
  };

  TestCountSort ts(std::move(ts_data), buffersData);

  TestBlockPipeline::TestBlockPipelineData tp_data{
      numGridPoints,
      layout,
      ts.getGridCounter(),
  };

  TestBlockPipeline tp(std::move(tp_data), buffersData);
  TestP2G::TestP2GData tp2g_data{
      ts.getSortedBufferData(),
      ts.getSortedBufferDataAccess(),
      tp.getBlockBufferData(),
      tp.getBlockBufferDataAccess(),
      gGridDim,
      tp.getIndirectDispatch(),
  };
  TestP2G tp2g(std::move(tp2g_data), buffersData);

  TestG2P::TestG2PData tg2p_data{
      ts.getSortedBufferData(),
      ts.getSortedBufferDataAccess(),
      tp.getBlockBufferData(),
      tp.getBlockBufferDataAccess(),
      gGridDim,
      tp.getIndirectDispatch(),
  };
  TestG2P tg2p(std::move(tg2p_data), buffersData);

  /**********************************************************************
   *                         execute dispatches                         *
   **********************************************************************/
  BenchmarkerCPU bench;
  bench.time("Total CPU time spent", [&ts, &tp, &tp2g, &tg2p, &numParticles,
                                      &numGridPoints]() {
    executeTest(1, [&ts, &tp, &tp2g, &tg2p, &numParticles, &numGridPoints]() {
      ts.run(numGridPoints, numParticles);
      tp.run(numGridPoints);
      tp2g.run(numGridPoints, numParticles);
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

