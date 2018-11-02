#ifndef PARTICLE_INDEX_BUFFER
#define PARTICLE_INDEX_BUFFER
#include "shader/shared_hpp/buffer_bindings.hpp"
#include "shader/buffers/structs/index.include.glsl"

layout(std430, binding = PARTICLE_INDICES_BINDING) buffer Indices_Buffer{
	Indices indices[];
};
#endif
