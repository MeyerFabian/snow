#include "TilePipeline.hpp"

void TilePipeline::init(TileData&& td_data, IOBufferData&& io) {
  GLuint tileSize = td_data.gGridDim.x * td_data.gGridDim.y *
                    td_data.gGridDim.z /
                    (VOXEL_DIM_X * VOXEL_DIM_Y * VOXEL_DIM_Z);

  /**********************************************************************
   *                          Buffer Creation                           *
   **********************************************************************/

  tile_buffer(BufferType::SSBO, BufferUsage::STATIC_DRAW, td_data.layout,
              "shader/buffers/tile.include.glsl");
  tile_buffer.resize_buffer(tileSize);
  tile_buffer.gl_bind_base(TILE_BINDING);
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

  IOBufferData io;
  // grid here
  io.in_buffer.push_back(std::make_unique<BufferData>(in_v));

  // tile counter here
  io.out_buffer.push_back(std::make_unique<BufferData>(out_g));

  MapReduceTechnique::MapReduceData reduce_data({
      "shader/compute/mapreduce/mapReduce.glsl",
      LocalSize{VOXEL_DIM_X * VOXEL_DIM_Y * VOXEL_DIM_Z, 1, 1},
      "uint",
      "SORTING_KEY(value)",
      "0",
      "max(left,right)",
  });

  std::vector<Shader::CommandType> reduce_commands = {};

  max_count.init(std::move(reduce_commands), std::move(reduce_data),
                 std::move(io));
}

void TilePipeline::run() {}

