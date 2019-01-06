#ifndef TESTBLOCKPIPELINE_HPP_7ZI58GST
#define TESTBLOCKPIPELINE_HPP_7ZI58GST

#include "../OutputBufferData.hpp"
#include "../block/BlockPipeline.hpp"
class TestBlockPipeline {
 public:
  struct TestBlockPipelineData {
    GLuint numGridPoints;
    BufferLayout layout;
    std::unique_ptr<BufferDataInterface> counter;
  };

  TestBlockPipeline(TestBlockPipelineData&& tp, OutputBufferData bd) {
#ifdef BLOCK_COMPACTION
    block_pipeline = std::make_shared<BlockPipeline>();

    BlockPipeline::BlockData block_data{
        tp.numGridPoints,
        tp.layout,
    };

    IOBufferData io_block;
    // in
    io_block.in_buffer.push_back(std::move(tp.counter));
    io_block.out_buffer.push_back(
        std::make_unique<BufferData>(bd.gridpoint_vel_mass));
#ifdef SHARED_PULL
    block_pipeline->initHalo(std::move(block_data), std::move(io_block));
#else
    block_pipeline->initBlock(std::move(block_data), std::move(io_block));
#endif
#endif
  }

  std::vector<std::unique_ptr<BlockBufferData> > getBlockBufferData() {
    return block_pipeline->getBlockBufferData();
  }

  std::vector<std::unique_ptr<BlockBufferDataAccess> >
  getBlockBufferDataAccess() {
    return block_pipeline->getBlockBufferDataAccess();
  }
  std::shared_ptr<IndirectDispatch> getIndirectDispatch() {
    return block_pipeline;
  }

  void run(GLuint numGridPoints) {
#ifdef BLOCK_COMPACTION
    block_pipeline->run({numGridPoints});
#endif
  }

 private:
  std::shared_ptr<BlockPipeline> block_pipeline;
};

#endif /* end of include guard: TESTBLOCKPIPELINE_HPP_7ZI58GST */

