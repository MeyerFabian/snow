#ifndef TILE_INDIRECT_DISPATCH
#define TILE_INDIRECT_DISPATCH
layout (binding = TILE_INDIRECT_BINDING) uniform TILE_INDIRECT_BUFFER {
	uint  num_groups_x;
	uint  num_groups_y;
	uint  num_groups_z;
};
#endif
