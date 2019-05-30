#version 440

// INPUT    offsets		  [N]
// INPUT2   scan on local level	  [M]
// INPUT3   scan on block level	  [M]
// INPUT3+X container to sort     [N] ->| Double-
// OUTPUTX  container to put sort [N] ->| Buffer prob.
//
// add.:
// INPUT4_VAR needs position variable
// and object of
// INPUT4 == OUTPUT
// INPUT5 == OUTPUT2
// ...
// etc.

#include "shader/shared_hpp/voxel_block_size.hpp"
#include "shader/utils/sorting_method.include.glsl"
#include "shader/buffers/grid_defines.include.glsl"

layout(local_size_x =X)in;


uniform uint bufferSize;

void main(void){
	uint unsortedIndex = gl_GlobalInvocationID.x;
	if(unsortedIndex>=bufferSize){
		return;
	}

#if INPUT4_SORTING_METHOD == INDEX_WRITE
	unsortedIndex = INPUT4_INDEX_AT(INPUT4_INDEX,INPUT4_INDEX_VAR,INPUT4_INDEX_SIZE,unsortedIndex,INPUT4_INDEX_NUM_BUFFER,INPUT4_INDEX_INDEX_BUFFER,INPUT4_INDEX_VAR_SIZE);
#endif
	PREC_VEC3_TYPE pos = INPUT4_AT(INPUT4,INPUT4_VAR,INPUT4_SIZE, unsortedIndex,INPUT4_NUM_BUFFER,INPUT4_INDEX_BUFFER,INPUT4_VAR_SIZE).xyz;

	// Bin due to position in grid
	PREC_VEC3_TYPE positionInGrid= (pos-grid_def.gGridPos)/grid_def.gridSpacing;

	//floor
	ivec3 globalGridIndex = ivec3(positionInGrid);


	if(inBounds(globalGridIndex,grid_def.gGridDim)){
		uint key = SORTING_KEY(globalGridIndex,grid_def.gGridDim);

		uint scanIndex =
			//scan_local
			INPUT2_AT(INPUT2,INPUT2_VAR,INPUT2_SIZE,get_scan_local_index(key),INPUT2_NUM_BUFFER,INPUT2_INDEX_BUFFER,INPUT2_VAR_SIZE)
#ifndef SCAN_DIRECT_WRITE_BACK
			+
			//scan_block
			INPUT3_AT(INPUT3,INPUT3_VAR,INPUT3_SIZE,get_scan_block_index(key),INPUT3_NUM_BUFFER,INPUT3_INDEX_BUFFER,INPUT3_VAR_SIZE)
#endif
			;
		uint scanOffset =
			INPUT_AT(INPUT,INPUT_VAR,INPUT_SIZE,unsortedIndex,INPUT_NUM_BUFFER,INPUT_INDEX_BUFFER,INPUT_VAR_SIZE);
		uint sortedIndex = scanIndex + scanOffset;

#if INPUT4_SORTING_METHOD == INDEX_WRITE || INPUT4_SORTING_METHOD == INDEX_READ

		OUTPUT_INDEX_AT(OUTPUT_INDEX,OUTPUT_INDEX_VAR,OUTPUT_INDEX_SIZE,sortedIndex,OUTPUT_INDEX_NUM_BUFFER,OUTPUT_INDEX_INDEX_BUFFER,OUTPUT_INDEX_VAR_SIZE) = unsortedIndex;
#else
		for(int var=0; var<OUTPUT_VAR_SIZE;var++){
			OUTPUT_AT(OUTPUT,var,OUTPUT_SIZE,sortedIndex,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER,OUTPUT_VAR_SIZE) = INPUT4_AT(INPUT4,var,INPUT4_SIZE,unsortedIndex,INPUT4_NUM_BUFFER,INPUT4_INDEX_BUFFER,INPUT4_VAR_SIZE);
		}
#ifdef OUTPUT2
		for(int var=0; var<OUTPUT2_VAR_SIZE;var++){
			OUTPUT2_AT(OUTPUT2,var,OUTPUT2_SIZE,sortedIndex,OUTPUT2_NUM_BUFFER,OUTPUT2_INDEX_BUFFER,OUTPUT2_VAR_SIZE) = INPUT5_AT(INPUT5,var,INPUT5_SIZE,unsortedIndex,INPUT5_NUM_BUFFER,INPUT5_INDEX_BUFFER,INPUT5_VAR_SIZE);
		}
#endif

#ifdef OUTPUT3
		for(int var=0; var<OUTPUT3_VAR_SIZE;var++){
			OUTPUT3_AT(OUTPUT3,var,OUTPUT3_SIZE,sortedIndex,OUTPUT3_NUM_BUFFER,OUTPUT3_INDEX_BUFFER,OUTPUT3_VAR_SIZE) = INPUT6_AT(INPUT6,var,INPUT6_SIZE,unsortedIndex,INPUT6_NUM_BUFFER,INPUT6_INDEX_BUFFER,INPUT6_VAR_SIZE);
		}
#endif
#endif
	}
}
