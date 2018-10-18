#version 440
layout(local_size_x =X, local_size_y =Y,local_size_z =Z)in;

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
 *
 * UNARY_OP(value) func(value)
 */

uniform uint bufferSize;

#ifndef INPUT_INDEX_BUFFER
#define INPUT_INDEX_BUFFER 0
#endif

#ifndef OUTPUT_INDEX_BUFFER
#define OUTPUT_INDEX_BUFFER 0
#endif

void main(void){
	uint i= gl_GlobalInvocationID.x;
	if(i<bufferSize)
		AT(OUTPUT,OUTPUT_VAR,OUTPUT_SIZE,i,OUTPUT_NUM_BUFFER,INPUT_INDEX_BUFFER) = UNARY_OP(AT(INPUT,INPUT_VAR,INPUT_SIZE,i,INPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER));
}
