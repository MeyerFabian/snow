#version 440
layout(local_size_x =X, local_size_y =Y,local_size_z =Z)in;

/*
 * Macros to be defined:
 *
 * INPUT buffer
 * INPUT_VAR var
 * OUTPUT buffer
 * OUTPUT_VAR var
 * where buffer needs to be included
 * e.g. AOS-Layout =>
 * AT(buffer,var,index) =>
 * buffer[index].var
 *
 * UNARY_OP(value) func(value)
 */

uniform uint bufferSize;

void main(void){
	uint i= gl_GlobalInvocationID.x;
	if(i<bufferSize)
		AT(OUTPUT,OUTPUT_VAR,i) = UNARY_OP(AT(INPUT,INPUT_VAR,i));
}
