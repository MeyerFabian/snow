#ifndef PARTICLE_OFFSET_BUFFER
#define PARTICLE_OFFSET_BUFFER
#include "shader/shared_hpp/buffer_bindings.hpp"
#include "shader/buffers/structs/gridOffset.include.glsl"

layout(std430, binding = PARTICLE_GRIDOFFSET_BINDING) buffer Particle_GridOffset_Buffer{
	Particle_GridOffset gridOffsets[];
};
#endif
