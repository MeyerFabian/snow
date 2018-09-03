#version 440
#define MAP(input) length(input)
layout(local_size_x =X, local_size_y =Y,local_size_z =Z)in;

struct Input{
	vec3 v;
};
struct Output{
	float f;
};

layout(std140, binding = 1) buffer vectors{
	Input b_in[ ];
};
layout(std140, binding = 2) buffer scalars{
	Output b_out[ ];
};


void main(void){
	uint i= gl_GlobalInvocationID.x;
	b_out[i].f = MAP(b_in[i].v);
}
