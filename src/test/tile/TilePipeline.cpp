#include "TilePipeline.hpp"

void TilePipeline::init(TileData&& td_data, IOBufferData&& io) {
  tileSize = td_data.numGridPoints / (VOXEL_DIM_X * VOXEL_DIM_Y * VOXEL_DIM_Z);

  /**********************************************************************
   *                          Buffer Creation                           *
   **********************************************************************/

  tile_buffer = std::make_unique<Buffer<Tile> >(
      BufferType::SSBO, BufferUsage::STATIC_DRAW, td_data.layout,
      "shader/buffers/tile.include.glsl");
  tile_buffer->resize_buffer(tileSize);
  tile_buffer->gl_bind_base(TILES_BINDING);
  /**********************************************************************
   *                        In/Outs for Shaders                         *
   **********************************************************************/

  auto tile_scan_local =
      BufferData("tiles", "Tile_scan_local", tile_buffer->get_buffer_info(),
                 tileSize, 1, "0", "Tile_VAR_SIZE");

  auto tile_scan_block =
      BufferData("tiles", "Tile_scan_block", tile_buffer->get_buffer_info(),
                 tileSize, 1, "0", "Tile_VAR_SIZE");

  tile_counter = std::make_unique<BufferData>(
      "tiles", "Tile_counter", tile_buffer->get_buffer_info(), tileSize, 1, "0",
      "Tile_VAR_SIZE");
  tile_index = std::make_unique<BufferData>("tiles", "Tile_index",
                                            tile_buffer->get_buffer_info(),
                                            tileSize, 1, "0", "Tile_VAR_SIZE");

  // tile counter here
  io.out_buffer.push_back(tile_counter->cloneBufferDataInterface());

  local_size = {
      VOXEL_DIM_X * VOXEL_DIM_Y * VOXEL_DIM_Z / global_loads_per_thread / 2, 1,
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
       "PERMUTATION(i) global_indexing_of_voxel_and_tile(i,grid_def.gGridDim)"},
      {PreprocessorCmd::ENDIF, ""},
  };

  max_count.init(std::move(reduce_commands), std::move(reduce_data),
                 std::move(io));

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
      tileSize,
      // raking
      2,
  };

  IOBufferData io_scan;

  // INPUT
  io_scan.in_buffer.push_back(tile_counter->cloneBufferDataInterface());

  // OUTPUT
  io_scan.out_buffer.push_back(tile_scan_local.cloneBufferDataInterface());

  // OUTPUT2
  io_scan.out_buffer.push_back(tile_scan_block.cloneBufferDataInterface());

  scanPipeline = ScanPipeline();

  scanPipeline.initDirectWriteBack(std::move(scan_data), std::move(io_scan));
}

void TilePipeline::run(GLuint numVectors) {
  GLuint buffer_size_before = numVectors;
  GLuint buffer_size_after =
      buffer_size_before / local_size.x / global_loads_per_thread;

  BenchmarkerGPU::getInstance().time(
      "tile_counter", [this, buffer_size_before, buffer_size_after]() {
        max_count.dispatch_with_barrier(
            {buffer_size_before, buffer_size_after, global_loads_per_thread});
      });

  scanPipeline.run(tileSize);
}

