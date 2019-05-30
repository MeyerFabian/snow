#ifndef BLOCK_PIPELINE_H
#define BLOCK_PIPELINE_H
#include <execution>

#include "../../../../../shader/shared_hpp/buffer_bindings.hpp"
#include "../../../../../shader/shared_hpp/interpolation_support.hpp"
#include "../../../../../shader/shared_hpp/voxel_block_size.hpp"
#include "../../../buffer/BufferData/BlockBufferData.hpp"
#include "../../../buffer/BufferData/BlockBufferDataAccess.hpp"
#include "../../../buffer/BufferData/BufferData.hpp"
#include "../../../buffer/BufferData/IOBufferData.hpp"
#include "../../../buffer/c_scan.hpp"
#include "../../../indirect_dispatch/IndirectDispatch.hpp"
#include "../../../object/grid/block.hpp"
#include "../../../simulation/computingTechnique/map/mapReduceTechnique.hpp"
#include "../../../simulation/computingTechnique/map/mapTechnique.hpp"
#include "../../../simulation/computingTechnique/reorder/reorderTechnique.hpp"
#include "../../../simulation/computingTechnique/scan/ScanPipeline.hpp"
#include "../../../simulation/computingTechnique/stream_compaction/streamCompactionTechnique.hpp"

#include "../../../object/grid/block_ind_dispatch.hpp"

/* A block pipeline consisting of:
 * 1. max reduction step to calculate maximum per-block count
 * 2. scan pipeline to calculate new tile indices for
 *   2.1 stores tile count in seperate buffer to start indirect dispatch
 * 3. a stream compaction for tile indices
 */
class BlockPipeline : public IndirectDispatch {
 public:
  struct BlockData {
    GLuint numGridPoints;
    BufferLayout layout;
  };

  struct BlockDispatchData {
    GLuint numGridPoints;
  };
  void run(GLuint numVectors);

  std::vector<std::unique_ptr<BlockBufferDataAccess>>
  getBlockBufferDataAccess();

  std::vector<std::unique_ptr<BlockBufferData>> getBlockBufferData();

  virtual void indirect_dispatch() override;

  void initBlock(BlockData&& td, IOBufferData&& io);
  void initHalo(BlockData&& td, IOBufferData&& io);

 private:
  void init(BlockData&& td, IOBufferData&& io);
  std::optional<GLuint> global_loads_per_thread;
  LocalSize local_size;
  MapReduceTechnique max_count;
  MapTechnique write_ind_disp_buffer;
  ScanPipeline scanPipeline;
  StreamCompactionTechnique streamCompaction;
  GLuint blockSize;
  std::unique_ptr<Buffer<DispatchIndirect>> indirect_dispatch_buffer;
  std::unique_ptr<Buffer<Block>> block_buffer;

  std::unique_ptr<BufferData> block_counter;
  std::unique_ptr<BufferData> block_index;
  std::unique_ptr<BufferData> block_dispatch_indirect;

  std::vector<std::unique_ptr<BlockBufferData>> block_data;
};
#endif

