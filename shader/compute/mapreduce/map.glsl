#version 440
layout(local_size_x =X, local_size_y =Y,local_size_z =Z)in;

/*
 * Macros to be defined:
 *
 * INPUT(id) in_buffer_var[id]
 * OUTPUT(id) out_buffer_var[id]
 * where buffer needs to be included
 *
 * UNARY_OP(value) func(value)
 */

void main(void){
	uint i= gl_GlobalInvocationID.x;
	OUTPUT(i) = UNARY_OP(INPUT(i));
}
