#include "BlockPipeline.hpp"

void BlockPipeline::indirect_dispatch() {
  indirect_dispatch_buffer->rebind_as(BufferType::DISPATCH);
  glDispatchComputeIndirect(0);
  indirect_dispatch_buffer->rebind();
}
void BlockPipeline::initBlock(BlockData&& td_data, IOBufferData&& io) {
  global_loads_per_thread = 1;
  blockSize = td_data.numGridPoints / (VOXEL_DIM_X * VOXEL_DIM_Y * VOXEL_DIM_Z);

  /**********************************************************************
   *                          Buffer Creation                           *
   **********************************************************************/

  block_buffer = std::make_unique<Buffer<Block> >(
      BufferType::SSBO, BufferUsage::STATIC_DRAW, td_data.layout,
      "shader/buffers/block.include.glsl");
  block_buffer->resize_buffer(blockSize);
  block_buffer->gl_bind_base(BLOCKS_BINDING);

  block_counter = std::make_unique<BufferData>(
      "blocks", "Block_counter", block_buffer->get_buffer_info(), blockSize, 1,
      "0", "Block_VAR_SIZE");

  IOBufferData io_map_reduce;
  io_map_reduce.in_buffer.push_back(
      io.in_buffer[0]->cloneBufferDataInterface());
  io_map_reduce.out_buffer.push_back(block_counter->cloneBufferDataInterface());

  local_size = {
      VOXEL_DIM_X * VOXEL_DIM_Y * VOXEL_DIM_Z / *global_loads_per_thread / 2, 1,
      1};

  MapReduceTechnique::MapReduceData reduce_data({
      "shader/compute/mapreduce/mapReduceNear.glsl",
      local_size,
      "uint",
      "value",
      "0",
      "max(left,right)",
  });

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
void BlockPipeline::initHalo(BlockData&& td_data, IOBufferData&& io) {
  blockSize = td_data.numGridPoints / (VOXEL_DIM_X * VOXEL_DIM_Y * VOXEL_DIM_Z);

  /**********************************************************************
   *                          Buffer Creation                           *
   **********************************************************************/

  block_buffer = std::make_unique<Buffer<Block> >(
      BufferType::SSBO, BufferUsage::STATIC_DRAW, td_data.layout,
      "shader/buffers/block.include.glsl");
  block_buffer->resize_buffer(blockSize);
  block_buffer->gl_bind_base(BLOCKS_BINDING);

  block_counter = std::make_unique<BufferData>(
      "blocks", "Block_counter", block_buffer->get_buffer_info(), blockSize, 1,
      "0", "Block_VAR_SIZE");

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
void BlockPipeline::init(BlockData&& td_data, IOBufferData&& io) {
  blockSize = td_data.numGridPoints / (VOXEL_DIM_X * VOXEL_DIM_Y * VOXEL_DIM_Z);

  /**********************************************************************
   *                          Buffer Creation                           *
   **********************************************************************/
  indirect_dispatch_buffer = std::make_unique<Buffer<DispatchIndirect> >(
      BufferType::SSBO, BufferUsage::STATIC_DRAW, BufferLayout::AOS,
      "shader/buffers/block_indirect_dispatch.include.glsl");
  indirect_dispatch_buffer->transfer_to_gpu(
      std::vector<DispatchIndirect>{{1, 1, 1}});
  indirect_dispatch_buffer->gl_bind_base(BLOCKS_INDIRECT_BINDING);

  /**********************************************************************
   *                        In/Outs for Shaders                         *
   **********************************************************************/

  auto block_scan_local =
      BufferData("blocks", "Block_scan_local", block_buffer->get_buffer_info(),
                 blockSize, 1, "0", "Block_VAR_SIZE");

  auto block_scan_block =
      BufferData("blocks", "Block_scan_block", block_buffer->get_buffer_info(),
                 blockSize, 1, "0", "Block_VAR_SIZE");

  block_index = std::make_unique<BufferData>(
      "blocks", "Block_index", block_buffer->get_buffer_info(), blockSize, 1,
      "0", "Block_VAR_SIZE");

  block_dispatch_indirect = std::make_unique<BufferData>(
      "dispatch_dim", "num_groups_x",
      indirect_dispatch_buffer->get_buffer_info(), 1, 1, "0", "0");
  // scan

  ScanTechnique::ScanData scan_data{
      // local_size
      {1024, 1, 1},
      // shader
      "shader/compute/preprocess/scan_unroll.glsl",
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

  // INPUT
  io_scan.in_buffer.push_back(block_counter->cloneBufferDataInterface());

  // OUTPUT
  io_scan.out_buffer.push_back(block_scan_local.cloneBufferDataInterface());

  // OUTPUT2
  io_scan.out_buffer.push_back(block_scan_block.cloneBufferDataInterface());

  scanPipeline = ScanPipeline();

  scanPipeline.initDirectWriteBack(std::move(scan_data), std::move(io_scan));

  write_ind_disp_buffer = MapTechnique();
  MapTechnique::MapData map_data{
      "shader/compute/mapreduce/mapSingle.glsl",
      // unary_op
      "value",
      1,
      LocalSize{1, 1, 1},
  };

  IOBufferData io_map;

  io_map.in_buffer.push_back(block_scan_local.cloneBufferDataInterface());
  io_map.in_buffer.push_back(block_counter->cloneBufferDataInterface());

  io_map.out_buffer.push_back(
      block_dispatch_indirect->cloneBufferDataInterface());

  std::string last_block = std::to_string(blockSize - 1);
  std::vector<Shader::CommandType> map_commands = {
      {PreprocessorCmd::DEFINE, "PERMUTATION_IN(i) " + last_block},
      {PreprocessorCmd::DEFINE, "UNARY_OP2(value) value"},
      {PreprocessorCmd::DEFINE, "BINARY_OP(x,y) x+y"},
  };
  write_ind_disp_buffer.init(std::move(map_commands), std::move(map_data),
                             std::move(io_map));

  // index create streamcompact
  StreamCompactionTechnique::StreamCompactionData stream_data{
      LocalSize{32, 1, 1},
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

  for (auto it = io.out_buffer.begin(); it != io.out_buffer.end(); it++) {
    auto block_buffer = std::make_unique<BlockBufferData>(
        (*it)->cloneBufferDataInterface(),
        BlockBufferData::IndexSSBOData{
            block_index->cloneBufferDataInterface()});
    block_data.push_back(std::move(block_buffer));
  }
}

void BlockPipeline::run(GLuint numVectors) {
  GLuint buffer_size_before = 0;
  GLuint buffer_size_after = 0;
  if (global_loads_per_thread) {
    // BLOCK_PIPELINE
    buffer_size_before = numVectors;
    buffer_size_after =
        buffer_size_before / local_size.x / (*global_loads_per_thread);
  } else {
    // HALO_PIPELINE
    buffer_size_after = blockSize;
  }
  BenchmarkerGPU::getInstance().time(
      "block_counter", [this, buffer_size_before, buffer_size_after]() {
        if (global_loads_per_thread) {
          max_count.dispatch_with_barrier(
              {buffer_size_before, buffer_size_after, global_loads_per_thread});
        } else {
          max_count.dispatch_with_barrier(
              {std::nullopt, buffer_size_after, std::nullopt,false});
        }
      });

  scanPipeline.run(blockSize);

  BenchmarkerGPU::getInstance().time(
      "indirect_dispatch_buffer_write",
      [this]() { write_ind_disp_buffer.dispatch_with_barrier({1}); });

  BenchmarkerGPU::getInstance().time("block_stream_compact", [this]() {
    streamCompaction.dispatch_with_barrier({blockSize});
  });
}

std::vector<std::unique_ptr<BlockBufferData> >
BlockPipeline::getBlockBufferData() {
  std::vector<std::unique_ptr<BlockBufferData> > ret;
  for (auto& buffer_data : block_data) {
    ret.push_back(buffer_data->clone());
  }
  return ret;
}
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

