#version 440

#extension GL_ARB_gpu_shader_fp64 : enable

layout(local_size_x =X)in;

/*
 * Copyright 1993-2009 NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property and
 * proprietary rights in and to this software and related documentation and
 * any modifications thereto.  Any use, reproduction, disclosure, or distribution
 * of this software and related documentation without an express license
 * agreement from NVIDIA Corporation is strictly prohibited.
 *
 */

/*
 * Macros to be defined:
 *
 * {INPUT,OUTPUT} buffer
 * {INPUT,OUTPUT}_VAR var
 * {INPUT,OUTPUT}_SIZE buffer
 * {INPUT,OUTPUT}_NUM_BUFFER double/multi buffer
 * {INPUT,OUTPUT}_INDEX_BUFFER which of the multi buffers
 *
 * where buffer needs to be included
 * e.g. AOS-Layout =>
 * AT(buffer,var,index) =>
 * buffer[index].var
 */
#ifndef PERMUTATION
#define PERMUTATION(i) i
#endif


shared UNARY_OP_RETURN_TYPE s_data[X];

uniform uint bufferSize;
uniform uint seq_loads;

void main(void){
	uint t_id = gl_LocalInvocationIndex;
	uint i = gl_WorkGroupID.x *seq_loads* X *2 + t_id;
	s_data[t_id] = BINARY_OP_NEUTRAL_ELEMENT;

	while(i < (gl_WorkGroupID.x+1) *seq_loads* X *2 && i<bufferSize){
		// if second seq. read out of bounds only do one
		if(i+X <bufferSize ){
			s_data[t_id] = BINARY_OP(
					s_data[t_id],
					BINARY_OP(
						UNARY_OP(
							INPUT_AT(INPUT,INPUT_VAR,INPUT_SIZE,PERMUTATION(i),INPUT_NUM_BUFFER,INPUT_INDEX_BUFFER)
							),
						UNARY_OP(
							INPUT_AT(INPUT,INPUT_VAR,INPUT_SIZE,PERMUTATION(i+X),INPUT_NUM_BUFFER,INPUT_INDEX_BUFFER)
							)
						)
					);
		}
		else {
			s_data[t_id] = BINARY_OP(
					s_data[t_id],
					UNARY_OP(
						INPUT_AT(INPUT,INPUT_VAR,INPUT_SIZE,i,INPUT_NUM_BUFFER,INPUT_INDEX_BUFFER)
						)
					);
		}
		//e.g. s_data[t_id] += g_data[i]+g_data[i+X];
		i+= X*2;
	}

	memoryBarrierShared();
	barrier();

#if X>=1024
	if(t_id<512){
		s_data[t_id] = BINARY_OP(
				s_data[t_id],
				s_data[t_id+512]
				);
		memoryBarrierShared();
		barrier();
	}
#endif

#if X>=512
	if(t_id<256){
		s_data[t_id] = BINARY_OP(
				s_data[t_id],
				s_data[t_id+256]
				);
		memoryBarrierShared();
		barrier();
	}
#endif
#if X>=256
	if(t_id<128){
		s_data[t_id] = BINARY_OP(
				s_data[t_id],
				s_data[t_id+128]
				);
		memoryBarrierShared();
		barrier();
	}
#endif
#if X>=128
	if(t_id<64){
		s_data[t_id] = BINARY_OP(
				s_data[t_id],
				s_data[t_id+64]
				);
		memoryBarrierShared();
		barrier();
	}
#endif

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


	if(t_id ==0) OUTPUT_AT(OUTPUT,OUTPUT_VAR,OUTPUT_SIZE,gl_WorkGroupID.x,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER) = s_data[0];
}
