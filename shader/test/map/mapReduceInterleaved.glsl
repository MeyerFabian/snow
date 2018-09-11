#version 440
layout(local_size_x =X)in;

/*
 * Macros to be defined:
 *
 * INPUT(id) in_buffer[id]
 * OUTPUT(id) out_buffer[id]
 * where buffer needs to be included
 *
 * UNARY_OP(value) func(value)
 * UNARY_OP_RETURN_TYPE
 */

shared UNARY_OP_RETURN_TYPE s_data[gl_WorkGroupSize.x];

void main(void){
	uint b_id = gl_WorkGroupID.x;
	uint b_size = gl_WorkGroupSize.x;
	uint t_id = gl_LocalInvocationIndex;
	uint g_id = gl_GlobalInvocationID.x;

	s_data[t_id] = UNARY_OP(INPUT(g_id));

	memoryBarrierShared();
	barrier();
	for(uint s=1; s < b_size; s *= 2) {
		if (t_id % (2*s) == 0) {
			s_data[t_id] += s_data[t_id + s];
		}
		memoryBarrierShared();
		barrier();
	}

	if(t_id ==0) OUTPUT(b_id) = s_data[0];
}
