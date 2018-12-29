#ifndef GRID_UNIFORMS
#define GRID_UNIFORMS

#define DIM_INDEX
#define SORTING_KEY get_dim_index
//#define SORTING_KEY get_voxel_and_tile_index

#include "shader/shared_hpp/buffer_bindings.hpp"
#include "shader/buffers/structs/grid_def.include.glsl"
#include "shader/compute/indexing/neighborIndex.include.glsl"
#include "shader/compute/indexing/gridIndex.include.glsl"
layout (binding = GRID_DEFINES_BINDING) uniform GRID_DEFINES_BUFFER {
	GridDefines grid_def;
};

#endif
