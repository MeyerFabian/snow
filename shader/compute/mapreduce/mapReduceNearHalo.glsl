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



shared UNARY_OP_RETURN_TYPE s_data[X];

//reverse supports
#define P2G_PULL
#include "shader/shared_hpp/interpolation_support.hpp"

#define HALO_X (VOXEL_DIM_X+LEFT_SUPPORT+RIGHT_SUPPORT)
#define HALO_Y (VOXEL_DIM_Y+LEFT_SUPPORT+RIGHT_SUPPORT)
#define HALO_Z (VOXEL_DIM_Z+LEFT_SUPPORT+RIGHT_SUPPORT)

#define blockProcessSize HALO_X*HALO_Y*HALO_Z
#define blockSize uvec3(VOXEL_DIM_X,VOXEL_DIM_Y,VOXEL_DIM_Z)
void main(void){
	uint t_id = gl_LocalInvocationIndex;
	s_data[t_id] = BINARY_OP_NEUTRAL_ELEMENT;


	/*****************************************/
	/*         alternate loading here        */
	/*****************************************/

	uint local_i = gl_LocalInvocationIndex;

	uvec3 blockID = getBlockID(gl_WorkGroupID.x,grid_def.gGridDim);
	ivec3 grid_start_node = ivec3(blockID * blockSize) - LEFT_SUPPORT;

	while(local_i < blockProcessSize){

		uvec3 halo_ijk = uvec3(getIJK(local_i,ivec3(HALO_X,HALO_Y,HALO_Z)));
		ivec3 to_process = grid_start_node + ivec3(halo_ijk);
		if(inBounds(to_process,grid_def.gGridDim)){
			uint read_key = SORTING_KEY(to_process,grid_def.gGridDim);

			s_data[t_id] = BINARY_OP(
					s_data[t_id],
					UNARY_OP(
						INPUT_AT(INPUT,INPUT_VAR,INPUT_SIZE,read_key,INPUT_NUM_BUFFER,INPUT_INDEX_BUFFER)
						)
					);
		}
		local_i+= X;
	}

	/*****************************************/
	/*             rest identical            */
	/*****************************************/

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
