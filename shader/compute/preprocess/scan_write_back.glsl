#version 440

// INPUT   scan on block level	  [M]
// OUTPUT  scan on local level    [N]

layout(local_size_x =X) in;
void main(){
	OUTPUT_AT(OUTPUT,OUTPUT_VAR,OUTPUT_SIZE,gl_GlobalInvocationID.x,INPUT_NUM_BUFFER,INPUT_INDEX_BUFFER) =
		BINARY_OP(
				OUTPUT_AT(OUTPUT,OUTPUT_VAR,OUTPUT_SIZE,gl_GlobalInvocationID.x,INPUT_NUM_BUFFER,INPUT_INDEX_BUFFER),
				INPUT_AT(INPUT,INPUT_VAR,INPUT_SIZE,get_scan_block_index(gl_GlobalInvocationID.x),INPUT_NUM_BUFFER,INPUT_INDEX_BUFFER)
			 );
}
