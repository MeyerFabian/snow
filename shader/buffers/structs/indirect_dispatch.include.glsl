#ifndef DISPATCH_INDIRECT_GLSL
#define DISPATCH_INDIRECT_GLSL
struct DispatchIndirect{
	GLuint  num_groups_x;
	GLuint  num_groups_y;
	GLuint  num_groups_z;
};
#endif
