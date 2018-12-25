#ifndef GRID_UNIFORMS
#define GRID_UNIFORMS
#define SORTING_KEY get_dim_index
#include "shader/buffers/structs/grid_def.include.glsl"
layout (binding = GRID_DEFINES_BINDING) uniform GRID_DEFINES_BUFFER {
	GridDefines grid_def;
};
#endif
