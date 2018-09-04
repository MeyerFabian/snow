#version 440
layout(local_size_x =X)in;

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

shared float s_data[gl_WorkGroupSize.x];

layout(std430, binding = 1) buffer vectors{
	Input g_in[ ];
};
layout(std430, binding = 2) buffer scalars{
	Output g_out[ ];
};

void main(void){
	uint b_id = gl_WorkGroupID.x;
	uint b_size = gl_WorkGroupSize.x;
	uint t_id = gl_LocalInvocationIndex;
	uint g_id = gl_GlobalInvocationID.x;

	s_data[t_id] = UNARY_OP(g_in[g_id].v);

	memoryBarrierShared();
	barrier();
	for(unsigned int s=1; s < b_size; s *= 2) {
		if (t_id % (2*s) == 0) {
			s_data[t_id] += s_data[t_id + s];
		}
		memoryBarrierShared();
		barrier();
	}

	if(t_id ==0) g_out[b_id].f = s_data[0];
}
