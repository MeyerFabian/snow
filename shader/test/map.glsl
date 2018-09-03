#version 440
layout(local_size_x =X, local_size_y =Y,local_size_z =Z)in;

/*
 * Macros to be defined:
 * UNARY_OP(value) func(value)
 */
struct Input{
	vec4 v;
};
struct Output{
	float f;
};

layout(std430, binding = 1) buffer vectors{
	Input b_in[ ];
};
layout(std430, binding = 2) buffer scalars{
	Output b_out[ ];
};


void main(void){
	uint i= gl_GlobalInvocationID.x;
	b_out[i].f = UNARY_OP(b_in[i].v);
}
