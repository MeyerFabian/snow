#version 440

//deprecated pretty much does not work with BinTechnique.cpp's dispatch call:
//problem is (dispatchSize/local_size.x + 1)

#include "shader/utils/sorting_method.include.glsl"
layout(local_size_x =X, local_size_y =Y,local_size_z =Z)in;

uniform uint bufferSize;
uniform uint dispatchSize;
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
PREC_VEC3_TYPE[MULTIPLE_ELEMENTS] pos;


void main(void){
	uint i = gl_GlobalInvocationID.x;
	uint j = 0;
	while(i<bufferSize){
		pos[j] = INPUT_AT(INPUT,INPUT_VAR,INPUT_SIZE,i,INPUT_NUM_BUFFER,INPUT_INDEX_BUFFER).xyz;
		i+= dispatchSize;
		j++;
	}

	i = gl_GlobalInvocationID.x;
	j=0;
	while(i<bufferSize){
		// Bin due to position in grid
		PREC_VEC3_TYPE positionInGrid= (pos[j]-grid_def.gGridPos)/grid_def.gridSpacing;

		//floor
		ivec3 globalGridIndex = ivec3(positionInGrid);
		if(inBounds(globalGridIndex,grid_def.gGridDim)){
			uint key = SORTING_KEY(globalGridIndex,grid_def.gGridDim);
#ifdef OUTPUT2
			OUTPUT2_AT(OUTPUT2,OUTPUT2_VAR,OUTPUT2_SIZE,i,OUTPUT2_NUM_BUFFER,OUTPUT2_INDEX_BUFFER) =
#endif
				atomicAdd(OUTPUT_AT(OUTPUT,OUTPUT_VAR,OUTPUT_SIZE,key,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER),1);
		}
		i+= dispatchSize;
		j++;
	}
}
