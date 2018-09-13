#version 440

#extension GL_ARB_gpu_shader_fp64 : enable

layout(local_size_x =X)in;

/*
 * Macros to be defined:
 *
 * INPUT(id) in_buffer[id]
 * OUTPUT(id) out_buffer[id]
 * where buffer needs to be included
 *
 * UNARY_OP(value) e.g. func(value)
 * UNARY_OP_RETURN_TYPE e.g. float
 * BINARY_OP(left,right) e.g. left+right
 */

shared UNARY_OP_RETURN_TYPE s_data[X];

uniform uint dispatchDim_x;
uniform uint bufferSize;

void main(void){
	uint t_id = gl_LocalInvocationIndex;
	uint i = gl_WorkGroupID.x * X *2 + t_id;
	uint dispatchSize = X * 2 * dispatchDim_x;
	s_data[t_id] = 0;

	while(i < bufferSize){
		s_data[t_id] = BINARY_OP(
				s_data[t_id],
				BINARY_OP(
					UNARY_OP(
						INPUT(i)
						),
					UNARY_OP(
						INPUT(i+X)
						)
					)
				);
		//e.g. s_data[t_id] += g_data[i]+g_data[i+X];
		i+= dispatchSize;
	}

	memoryBarrierShared();
	barrier();

	for(uint s=X/2; s > 32; s >>= 1) {
		if (t_id < s) {
			s_data[t_id] = BINARY_OP(
					s_data[t_id],
					s_data[t_id + s]
					);
		}
		memoryBarrierShared();
		barrier();
	}
	if(t_id<32){
#if X>=64
		s_data[t_id] = BINARY_OP(
				s_data[t_id],
				s_data[t_id+32]
				);
		memoryBarrierShared();
#endif
#if X>=32
		s_data[t_id] = BINARY_OP(
				s_data[t_id],
				s_data[t_id+16]
				);
		memoryBarrierShared();
#endif
#if X>=16
		s_data[t_id] = BINARY_OP(
				s_data[t_id],
				s_data[t_id+8]
				);
		memoryBarrierShared();
#endif
#if X>=8
		s_data[t_id] = BINARY_OP(
				s_data[t_id],
				s_data[t_id+4]
				);
		memoryBarrierShared();
#endif
#if X>=4
		s_data[t_id] = BINARY_OP(
				s_data[t_id],
				s_data[t_id+2]
				);
		memoryBarrierShared();
#endif
#if X>=2
		s_data[t_id] = BINARY_OP(
				s_data[t_id],
				s_data[t_id+1]
				);
		memoryBarrierShared();
#endif
	}


	if(t_id ==0) OUTPUT(gl_WorkGroupID.x) = s_data[0];
}
