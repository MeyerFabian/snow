#ifndef BLOCK_INDIRECT_DISPATCH
#define BLOCK_INDIRECT_DISPATCH

#include "shader/buffers/structs/indirect_dispatch.include.glsl"
layout (std430, binding = BLOCKS_INDIRECT_BINDING) buffer BLOCKS_INDIRECT_BUFFER {
	DispatchIndirect dispatch_dim[];
};
#endif
