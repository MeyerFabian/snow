#include "BlockPipeline.hpp"

// starts a gpu dispatch with count of active tiles
void BlockPipeline::indirect_dispatch() {
  indirect_dispatch_buffer->rebind_as(BufferType::DISPATCH);
  glDispatchComputeIndirect(0);
  indirect_dispatch_buffer->rebind();
}

/* Initializes the block pipeline for blocks excluding the halo, p2g-atomic and
 * p2g-sync use this since they write into the halo but stay within the block
 *
 * @IOBufferData: in_buffer[0] expects particles per cell counter
 */
void BlockPipeline::initBlock(BlockData&& td_data, IOBufferData&& io) {
  global_loads_per_thread = 1;
  // the amount of blocks is voxels divided by the voxels per block
  blockSize = td_data.numGridPoints / (VOXEL_DIM_X * VOXEL_DIM_Y * VOXEL_DIM_Z);

  // create a block buffer for block count, scan and index
  block_buffer = std::make_unique<Buffer<Block> >(
      BufferType::SSBO, BufferUsage::STATIC_DRAW, td_data.layout,
      "shader/buffers/block.include.glsl");
  block_buffer->resize_buffer(blockSize);
  block_buffer->gl_bind_base(BLOCKS_BINDING);

  // stores buffer information for per block maximum counter for mpm-transfers
  block_counter = std::make_unique<BufferData>(
      "blocks", "Block_counter", block_buffer->get_buffer_info(), blockSize, 1,
      "0", "Block_VAR_SIZE");

  IOBufferData io_map_reduce;
  // in: particles per cell count in here
  io_map_reduce.in_buffer.push_back(
      io.in_buffer[0]->cloneBufferDataInterface());
  // out: maximum particle count per block
  io_map_reduce.out_buffer.push_back(block_counter->cloneBufferDataInterface());

  // one thread block is spanned by voxels in three dimensions
  local_size = {
      VOXEL_DIM_X * VOXEL_DIM_Y * VOXEL_DIM_Z / *global_loads_per_thread / 2, 1,
      1};

  // maximum reduction
  MapReduceTechnique::MapReduceData reduce_data({
      "shader/compute/mapreduce/mapReduceNear.glsl",
      local_size,
      "uint",
      "value",
      "0",
      "max(left,right)",
  });

  // reduction needs to reduce values within a block
  // global_indexing_of_voxel_and_block addresses this index change
  std::vector<Shader::CommandType> reduce_commands = {
      {PreprocessorCmd::IFDEF, "DIM_INDEX"},
      {PreprocessorCmd::DEFINE,
       "PERMUTATION(i) "
       "global_indexing_of_voxel_and_block(i,grid_def.gGridDim)"},
      {PreprocessorCmd::ENDIF, ""},
  };

  max_count.init(std::move(reduce_commands), std::move(reduce_data),
                 std::move(io_map_reduce));

  init(std::move(td_data), std::move(io));
}

/* Initializes a block pipeline for blocks including the halo, p2g-pull uses
 * this as the whole halo needs to be read from
 *
 * @IOBufferData: in_buffer[0] expects particles per cell counter
 */
void BlockPipeline::initHalo(BlockData&& td_data, IOBufferData&& io) {
  blockSize = td_data.numGridPoints / (VOXEL_DIM_X * VOXEL_DIM_Y * VOXEL_DIM_Z);

  // Create a block buffer for block count, scan and index
  block_buffer = std::make_unique<Buffer<Block> >(
      BufferType::SSBO, BufferUsage::STATIC_DRAW, td_data.layout,
      "shader/buffers/block.include.glsl");
  block_buffer->resize_buffer(blockSize);
  block_buffer->gl_bind_base(BLOCKS_BINDING);

  // stores buffer information for per block maximum counter for mpm-transfers
  block_counter = std::make_unique<BufferData>(
      "blocks", "Block_counter", block_buffer->get_buffer_info(), blockSize, 1,
      "0", "Block_VAR_SIZE");

  // maximum reduction within halo: halo elements get handled multiple times,
  // i.e. for each block they are in
  IOBufferData io_map_reduce;
  io_map_reduce.in_buffer.push_back(
      io.in_buffer[0]->cloneBufferDataInterface());
  io_map_reduce.out_buffer.push_back(block_counter->cloneBufferDataInterface());

  local_size = {VOXEL_DIM_X * VOXEL_DIM_Y * VOXEL_DIM_Z, 1, 1};

  MapReduceTechnique::MapReduceData reduce_data({
      "shader/compute/mapreduce/mapReduceNearHalo.glsl",
      local_size,
      "uint",
      "value",
      "0",
      "max(left,right)",
  });
  std::vector<Shader::CommandType> reduce_commands = {
      /*
            {PreprocessorCmd::IFDEF, "DIM_INDEX"},
            {PreprocessorCmd::DEFINE,
             "PERMUTATION(i) "
             "global_indexing_of_voxel_and_block(i,grid_def.gGridDim)"},
            {PreprocessorCmd::ENDIF, ""},
        */
  };

  max_count.init(std::move(reduce_commands), std::move(reduce_data),
                 std::move(io_map_reduce));

  init(std::move(td_data), std::move(io));
}

/*
 * Initializes block pipeline buffers & techniques(shaders)
 *
 * @IOBufferData: in_buffer[0] expects particles per cell counter, due to
 * calling initHalo or initBlock
 */
void BlockPipeline::init(BlockData&& td_data, IOBufferData&& io) {
  blockSize = td_data.numGridPoints / (VOXEL_DIM_X * VOXEL_DIM_Y * VOXEL_DIM_Z);

  /**********************************************************************
   *                          Buffer Creation                           *
   **********************************************************************/

  // If a block has atleast one particle, it counts as active. This buffer holds
  // the amount of active blocks.
  indirect_dispatch_buffer = std::make_unique<Buffer<DispatchIndirect> >(
      BufferType::SSBO, BufferUsage::STATIC_DRAW, BufferLayout::AOS,
      "shader/buffers/block_indirect_dispatch.include.glsl");
  indirect_dispatch_buffer->transfer_to_gpu(
      std::vector<DispatchIndirect>{{1, 1, 1}});
  indirect_dispatch_buffer->gl_bind_base(BLOCKS_INDIRECT_BINDING);

  /**********************************************************************
   *                        In/Outs for Shaders                         *
   **********************************************************************/

  // scan for active blocks
  auto block_scan_local =
      BufferData("blocks", "Block_scan_local", block_buffer->get_buffer_info(),
                 blockSize, 1, "0", "Block_VAR_SIZE");

  auto block_scan_block =
      BufferData("blocks", "Block_scan_block", block_buffer->get_buffer_info(),
                 blockSize, 1, "0", "Block_VAR_SIZE");

  // out of the scan a new indexing for active blocks is generated
  block_index = std::make_unique<BufferData>(
      "blocks", "Block_index", block_buffer->get_buffer_info(), blockSize, 1,
      "0", "Block_VAR_SIZE");

  block_dispatch_indirect = std::make_unique<BufferData>(
      "dispatch_dim", "num_groups_x",
      indirect_dispatch_buffer->get_buffer_info(), 1, 1, "0", "0");

  ScanTechnique::ScanData scan_data{
      // local_size
      {1024, 1, 1},
      // shader
      "shader/compute/scan/scan_unroll.glsl",
      // unary_op_return_type
      "uint",
      // unary_op
      "((value>0)?1:0)",
      // gl_binary_op_neutral_elem
      "0",
      // gl_biunary_op
      "left+right",
      blockSize,
      // raking
      2,
  };

  IOBufferData io_scan;

  // INPUT for the scan is the per block maximum particle count
  io_scan.in_buffer.push_back(block_counter->cloneBufferDataInterface());

  // OUTPUT will be the scan
  io_scan.out_buffer.push_back(block_scan_local.cloneBufferDataInterface());

  // OUTPUT2
  io_scan.out_buffer.push_back(block_scan_block.cloneBufferDataInterface());

  scanPipeline = ScanPipeline();

  // direct write back ensures everything is written back into block_scan_local
  scanPipeline.initDirectWriteBack(std::move(scan_data), std::move(io_scan));

  /* The last integer in the scan(1.) + the last per block maximum particle
   * count(2.) result in the total number of active blocks. The GPU needs to
   * copy this over into a seperate buffer(3.) s.t. we can call
   * glDispatchIndirect(...).
   */
  write_ind_disp_buffer = MapTechnique();
  MapTechnique::MapData map_data{
      "shader/compute/map/map.glsl",
      // unary_op
      "value",
      1,
      LocalSize{1, 1, 1},
  };

  IOBufferData io_map;
  // INPUT : per block scan           (1.)
  io_map.in_buffer.push_back(block_scan_local.cloneBufferDataInterface());
  // INPUT2: per block counter        (2.)
  io_map.in_buffer.push_back(block_counter->cloneBufferDataInterface());
  // OUTPUT: indirect dispatch buffer (3.)
  io_map.out_buffer.push_back(
      block_dispatch_indirect->cloneBufferDataInterface());

  // Modify map to be a an addition (1. + 2.) and a copy into (3.)
  std::string last_block = std::to_string(blockSize - 1);
  std::vector<Shader::CommandType> map_commands = {
      {PreprocessorCmd::DEFINE, "PERMUTATION_IN(i) " + last_block},
      {PreprocessorCmd::DEFINE, "UNARY_OP2(value) value"},
      {PreprocessorCmd::DEFINE, "BINARY_OP(x,y) x+y"},
  };
  write_ind_disp_buffer.init(std::move(map_commands), std::move(map_data),
                             std::move(io_map));

  /* Stream compaction or filter is responsible for writing only the indices of
   * the active tiles into block_index. The scan of active blocks tells us where
   * to write.
   *
   * e.g.:
   * | block         | 0 | 1 | 2 | 3 | 4 | 5 |
   * | active?       | 0 | 1 | 0 | 1 | 1 | 1 |
   * | scan          | 0 | 0 | 1 | 1 | 2 | 3 |
   *   result:
   * | active index  | 1 | 3 | 4 | 5 |
   */
  StreamCompactionTechnique::StreamCompactionData stream_data{
      LocalSize{32, 1, 1},
      // active check: block not empty?
      "((value>0)?true:false)",
  };

  IOBufferData io_stream_compact;
  io_stream_compact.in_buffer.push_back(
      block_counter->cloneBufferDataInterface());
  io_stream_compact.in_buffer.push_back(
      block_scan_local.cloneBufferDataInterface());
  io_stream_compact.out_buffer.push_back(
      block_index->cloneBufferDataInterface());

  streamCompaction = StreamCompactionTechnique();
  streamCompaction.init(std::move(stream_data), std::move(io_stream_compact));

  // Make the block_index available for other techniques to query(*)
  for (auto it = io.out_buffer.begin(); it != io.out_buffer.end(); it++) {
    auto block_buffer = std::make_unique<BlockBufferData>(
        (*it)->cloneBufferDataInterface(),
        BlockBufferData::IndexSSBOData{
            block_index->cloneBufferDataInterface()});
    block_data.push_back(std::move(block_buffer));
  }
}

// (*) can be thus queried by getter
std::vector<std::unique_ptr<BlockBufferData> >
BlockPipeline::getBlockBufferData() {
  std::vector<std::unique_ptr<BlockBufferData> > ret;
  for (auto& buffer_data : block_data) {
    ret.push_back(buffer_data->clone());
  }
  return ret;
}
// access getter/class also makes the block_counter available for other shaders
std::vector<std::unique_ptr<BlockBufferDataAccess> >
BlockPipeline::getBlockBufferDataAccess() {
  std::vector<std::unique_ptr<BlockBufferDataAccess> > ret;
  for (auto& buffer_data : block_data) {
    ret.push_back(std::make_unique<BlockBufferDataAccess>(
        buffer_data->clone(), BlockBufferDataAccess::IndexSSBOData{
                                  block_counter->cloneBufferDataInterface(),
                              }));
  }
  return ret;
}

/*
 * Executed every physical timestep.
 */
void BlockPipeline::run(GLuint numVectors) {
  GLuint buffer_size_before = 0;
  GLuint buffer_size_after = 0;
  if (global_loads_per_thread) {  // meh, check for block or halo TODO: rework
    // BLOCK_PIPELINE
    buffer_size_before = numVectors;
    buffer_size_after =
        buffer_size_before / local_size.x / (*global_loads_per_thread);
  } else {
    // HALO_PIPELINE
    buffer_size_after = blockSize;
  }
  // (1.) max reduction particle cell counter to per block maximum cell counter
  BenchmarkerGPU::getInstance().time(
      "block_counter", [this, buffer_size_before, buffer_size_after]() {
        if (global_loads_per_thread) {  // same here TODO: rework
          // BLOCK
          max_count.dispatch_with_barrier(
              {buffer_size_before, buffer_size_after, global_loads_per_thread});
        } else {
          // HALO
          max_count.dispatch_with_barrier(
              {std::nullopt, buffer_size_after, std::nullopt, false});
        }
      });
  // (2.) scan
  scanPipeline.run(blockSize);

  // (2.1) copy to indirect dispatch buffer
  BenchmarkerGPU::getInstance().time(
      "indirect_dispatch_buffer_write",
      [this]() { write_ind_disp_buffer.dispatch_with_barrier({1}); });

  // (3.) filter/stream compact active indices
  BenchmarkerGPU::getInstance().time("block_stream_compact", [this]() {
    streamCompaction.dispatch_with_barrier({blockSize});
  });
}

