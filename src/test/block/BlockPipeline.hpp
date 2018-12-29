#ifndef BLOCK_PIPELINE_H
#define BLOCK_PIPELINE_H
#include <execution>

#include "../../../shader/shared_hpp/buffer_bindings.hpp"
#include "../../../shader/shared_hpp/voxel_block_size.hpp"
#include "../../src/snow/buffer/c_scan.hpp"
#include "../../src/snow/grid/block.hpp"
#include "../../test/map/mapReduceTechnique.hpp"
#include "../../test/map/mapTechnique.hpp"
#include "../../test/reorder/reorderTechnique.hpp"
#include "../../test/stream_compaction/streamCompactionTechnique.hpp"
#include "../BlockBufferData.hpp"
#include "../BlockBufferDataAccess.hpp"
#include "../BufferData.hpp"
#include "../IOBufferData.hpp"
#include "../scan/ScanPipeline.hpp"

#include "../../src/snow/grid/block_ind_dispatch.hpp"
class BlockPipeline {
 public:
  struct BlockData {
    GLuint numGridPoints;
    BufferLayout layout;
  };

  struct BlockDispatchData {
    GLuint numGridPoints;
  };
  void init(BlockData&& td, IOBufferData&& io);
  void run(GLuint numVectors);

  std::vector<std::unique_ptr<BlockBufferDataAccess>>
  getBlockBufferDataAccess();

  std::vector<std::unique_ptr<BlockBufferData>> getBlockBufferData();

 private:
  GLuint global_loads_per_thread = 1;
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

