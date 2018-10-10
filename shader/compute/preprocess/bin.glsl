#version 440
#include "shader/shared_hpp/voxel_tile_size.hpp"
#include "shader/compute/indexing/gridIndex.include.glsl"
layout(local_size_x =X, local_size_y =Y,local_size_z =Z)in;
uniform uvec3 gGridDim;
uniform PREC_VEC3_TYPE gGridPos;
uniform PREC_SCAL_TYPE gridSpacing;

uniform uint bufferSize;

void main(void){
	uint i = gl_GlobalInvocationID.x;
	if(i>=bufferSize){
		return;
	}
	PREC_VEC3_TYPE pos = AT(INPUT,INPUT_VAR,i).xyz;

	// Bin due to position in grid
	PREC_VEC3_TYPE positionInGrid= (pos-gGridPos)/gridSpacing;

	//floor
	ivec3 globalGridIndex = ivec3(positionInGrid);
	if(inBounds(globalGridIndex,gGridDim)){
		uint voxelAndTileIndex = get_voxel_and_tile_index(globalGridIndex,gGridDim);
#ifdef OUTPUT2
		AT(OUTPUT2,OUTPUT2_VAR,i) =
#endif
			atomicAdd(AT(OUTPUT,OUTPUT_VAR,voxelAndTileIndex),1);
	}
}
