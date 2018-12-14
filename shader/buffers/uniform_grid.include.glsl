#ifndef UNIFORM_GRID_BUFFER
#define UNIFORM_GRID_BUFFER
#include "shader/shared_hpp/buffer_bindings.hpp"
#include "shader/buffers/structs/grid.include.glsl"
layout(std430, binding = UNIFORM_GRID_BINDING) buffer UniformGrid{
	Gridpoint gridpoints[];
};
#endif
