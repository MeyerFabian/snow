#ifndef GRID_OFFSET_BUFFER
#define GRID_OFFSET_BUFFER
#include "shader/shared_hpp/buffer_bindings.hpp"
#include "shader/buffers/structs/gridOffset.include.glsl"

layout(std430, binding = GRID_OFFSET_BINDING) buffer Particle_GridOffset{
	GridOffset gridOffsets[];
};
#endif
