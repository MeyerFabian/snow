#version 440
#include "shader/shared_hpp/voxel_tile_size.hpp"
#include "shader/compute/indexing/gridIndex.include.glsl"
layout(local_size_x =X, local_size_y =Y,local_size_z =Z)in;
uniform uvec3 gGridDim;
uniform PREC_VEC3_TYPE gGridPos;
uniform PREC_SCAL_TYPE gridSpacing;

uniform uint bufferSize;

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

#ifndef INPUT_INDEX_BUFFER
#define INPUT_INDEX_BUFFER 0
#endif

#ifndef OUTPUT_INDEX_BUFFER
#define OUTPUT_INDEX_BUFFER 0
#endif


#ifndef OUTPUT2_INDEX_BUFFER
#define OUTPUT2_INDEX_BUFFER 0
#endif


void main(void){
	uint i = gl_GlobalInvocationID.x;
	if(i>=bufferSize){
		return;
	}
	PREC_VEC3_TYPE pos = INPUT_AT(INPUT,INPUT_VAR,INPUT_SIZE,i,INPUT_NUM_BUFFER,INPUT_INDEX_BUFFER).xyz;

	// Bin due to position in grid
	PREC_VEC3_TYPE positionInGrid= (pos-gGridPos)/gridSpacing;

	//floor
	ivec3 globalGridIndex = ivec3(positionInGrid);
	if(inBounds(globalGridIndex,gGridDim)){
		uint voxelAndTileIndex = get_voxel_and_tile_index(globalGridIndex,gGridDim);
#ifdef OUTPUT2
		OUTPUT2_AT(OUTPUT2,OUTPUT2_VAR,OUTPUT2_SIZE,i,OUTPUT2_NUM_BUFFER,OUTPUT2_INDEX_BUFFER) =
#endif
			atomicAdd(OUTPUT_AT(OUTPUT,OUTPUT_VAR,OUTPUT_SIZE,voxelAndTileIndex,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER),1);
	}
}
