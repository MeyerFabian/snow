#version 440

// INPUT    UNARY_OP(INPUT) = bool [M]
// INPUT2   scan on local level	   [M]
// OUTPUT   index buffer[N]
//
// UNARY_OP
//
// index only atm
//
layout(local_size_x =X)in;

uniform uint bufferSize;

void main(void){
	uint i = gl_GlobalInvocationID.x;
	if(i>=bufferSize){
		return;
	}

	bool should_write = UNARY_OP(INPUT_AT(INPUT,INPUT_VAR,INPUT_SIZE,i,INPUT_NUM_BUFFER,INPUT_INDEX_BUFFER,INPUT_VAR_SIZE));

	if(should_write){

		uint scanIndex =
			INPUT2_AT(INPUT2,INPUT2_VAR,INPUT2_SIZE,i,INPUT2_NUM_BUFFER,INPUT2_INDEX_BUFFER,INPUT2_VAR_SIZE);

		OUTPUT_AT(OUTPUT,OUTPUT_VAR,OUTPUT_SIZE,scanIndex,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER,OUTPUT_VAR_SIZE) = i;
	}
}
