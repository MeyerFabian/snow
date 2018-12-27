#ifndef TILE_BUFFER
#define TILE_BUFFER
#include "shader/defines/sorting_key.include.glsl"
#include "shader/shared_hpp/buffer_bindings.hpp"
#include "shader/buffers/structs/tile.include.glsl"
#include "shader/compute/indexing/scan_block_index.glsl"
layout(std430, binding = TILES_BINDING) buffer Tiles{
	Tile tiles[];
};
#endif
