#ifndef TILE_PIPELINE_H
#define TILE_PIPELINE_H
#include <execution>

#include "../../../shader/shared_hpp/buffer_bindings.hpp"
#include "../../../shader/shared_hpp/voxel_tile_size.hpp"
#include "../../src/snow/buffer/c_scan.hpp"
#include "../../src/snow/grid/tile.hpp"
#include "../../test/map/mapReduceTechnique.hpp"
#include "../../test/reorder/reorderTechnique.hpp"
#include "../BufferData.hpp"
#include "../IOBufferData.hpp"
class TilePipeline {
 public:
  struct TileData {
    BufferLayout layout;
    PREC_VEC3_TYPE gGridPos;
    glm::uvec3 gGridDim;
    PREC_SCAL_TYPE gridSpacing;
  };

  struct TileDispatchData {
    GLuint numGridPoints;
  };
  void init(TileData&& td, IOBufferData&& io);
  void run(GLuint numVectors);

 private:
  GLuint global_loads_per_thread = 1;
  LocalSize local_size;
  MapReduceTechnique max_count;
  std::unique_ptr<Buffer<Tile>> tile_buffer;
  std::unique_ptr<BufferData> tile_counter;
  std::unique_ptr<BufferData> tile_index;
};
#endif

