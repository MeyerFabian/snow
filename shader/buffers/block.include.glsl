#ifndef BLOCK_BUFFER
#define BLOCK_BUFFER
#include "shader/buffers/grid_defines.include.glsl"
#include "shader/shared_hpp/buffer_bindings.hpp"
#include "shader/buffers/structs/block.include.glsl"
#include "shader/compute/indexing/scan_block_index.glsl"
layout(std430, binding = BLOCKS_BINDING) buffer Blocks{
	Block blocks[];
};
#endif
