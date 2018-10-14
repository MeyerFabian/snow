#version 440
layout(local_size_x =X)in;

/*
 * Macros to be defined:
 *
 * INPUT(id) in_buffer[id]
 * OUTPUT(id) out_buffer[id]
 * where buffer needs to be included
 *
 * UNARY_OP(value) length(value)
 * UNARY_OP_RETURN_TYPE float
 * BINARY_OP_NEUTRAL_ELEMENT 0
 * BINARY_OP(value) a+b
 */

// start with half the dispatch size
shared UNARY_OP_RETURN_TYPE s_data[X*2];

// i will prob only use +

uniform uint bufferSize;

void main(void){
	uint tIndex = gl_LocalInvocationIndex;
	uint leftThreadIndex = 2 * tIndex;
	uint rightThreadIndex = 2 * tIndex +1;
	uint globalIndexLeft = MULTIPLE_ELEMENTS*(gl_WorkGroupID.x * X * 2 + leftThreadIndex);
	uint globalIndexRight = MULTIPLE_ELEMENTS*(gl_WorkGroupID.x * X * 2 + rightThreadIndex);

	if(globalIndexLeft > bufferSize) return;
	// raking sequantial global loads all values in raking get stored in registers for global writes at end
	uint[MULTIPLE_ELEMENTS] leftRaking;
	uint[MULTIPLE_ELEMENTS] rightRaking;


	leftRaking[0] = 0;
	rightRaking[0] = 0;

	for(int j = 0; j < MULTIPLE_ELEMENTS-1; j++) {
		leftRaking[j+1] = BINARY_OP(leftRaking[j] , UNARY_OP(AT(INPUT,INPUT_VAR,globalIndexLeft+j)));
		rightRaking[j+1] = BINARY_OP(rightRaking[j] , UNARY_OP(AT(INPUT,INPUT_VAR,globalIndexRight+j)));
	}
	// put partial reduced result in shared data
	s_data[leftThreadIndex] = BINARY_OP(
			leftRaking[MULTIPLE_ELEMENTS-1],
			UNARY_OP(AT(INPUT,INPUT_VAR,globalIndexLeft + MULTIPLE_ELEMENTS - 1))
			);
	s_data[rightThreadIndex] =BINARY_OP(
			rightRaking[MULTIPLE_ELEMENTS-1],
			UNARY_OP(AT(INPUT,INPUT_VAR,globalIndexRight + MULTIPLE_ELEMENTS - 1))
			);

	//interleaved parallel reduction with reversed indices
	//tree up-sweep (we start at leaves, d= max_depth(tree))
	//
	//

#if X>=1024
	barrier();
	memoryBarrierShared();
	if(tIndex < 1024){
		uint leftSharedIndex  = 1*(leftThreadIndex+1)-1;
		uint rightSharedIndex = 1*(rightThreadIndex+1)-1;
		s_data[rightSharedIndex] = BINARY_OP(s_data[rightSharedIndex],s_data[leftSharedIndex]);
	}
#endif

#if X>=512
	barrier();
	memoryBarrierShared();
	if(tIndex < 512){
		uint leftSharedIndex  = 2*(leftThreadIndex+1)-1;
		uint rightSharedIndex = 2*(rightThreadIndex+1)-1;
		s_data[rightSharedIndex] = BINARY_OP(s_data[rightSharedIndex],s_data[leftSharedIndex]);
	}
#endif


#if X>=256
	barrier();
	memoryBarrierShared();
	if(tIndex < 256){
		uint leftSharedIndex  = 4*(leftThreadIndex+1)-1;
		uint rightSharedIndex = 4*(rightThreadIndex+1)-1;
		s_data[rightSharedIndex] = BINARY_OP(s_data[rightSharedIndex],s_data[leftSharedIndex]);
	}
#endif

#if X>=128
	barrier();
	memoryBarrierShared();
	if(tIndex < 128){
		uint leftSharedIndex  = 8*(leftThreadIndex+1)-1;
		uint rightSharedIndex = 8*(rightThreadIndex+1)-1;
		s_data[rightSharedIndex] = BINARY_OP(s_data[rightSharedIndex],s_data[leftSharedIndex]);
	}
#endif

#if X>=64
	barrier();
	memoryBarrierShared();
	if(tIndex < 64){
		uint leftSharedIndex  = 16*(leftThreadIndex+1)-1;
		uint rightSharedIndex = 16*(rightThreadIndex+1)-1;
		s_data[rightSharedIndex] = BINARY_OP(s_data[rightSharedIndex],s_data[leftSharedIndex]);
	}
#endif

#if X>=32
	barrier();
	memoryBarrierShared();
	if(tIndex < 32){
		uint leftSharedIndex  = 32*(leftThreadIndex+1)-1;
		uint rightSharedIndex = 32*(rightThreadIndex+1)-1;
		s_data[rightSharedIndex] = BINARY_OP(s_data[rightSharedIndex],s_data[leftSharedIndex]);
	}
#endif

#if X>=16
	memoryBarrierShared();
	if(tIndex < 16){
		uint leftSharedIndex  = 64*(leftThreadIndex+1)-1;
		uint rightSharedIndex = 64*(rightThreadIndex+1)-1;
		s_data[rightSharedIndex] = BINARY_OP(s_data[rightSharedIndex],s_data[leftSharedIndex]);
	}
#endif

#if X>=8
	memoryBarrierShared();
	if(tIndex < 8){
		uint leftSharedIndex  = 128*(leftThreadIndex+1)-1;
		uint rightSharedIndex = 128*(rightThreadIndex+1)-1;
		s_data[rightSharedIndex] = BINARY_OP(s_data[rightSharedIndex],s_data[leftSharedIndex]);
	}
#endif


#if X>=4
	memoryBarrierShared();
	if(tIndex < 4){
		uint leftSharedIndex  = 256*(leftThreadIndex+1)-1;
		uint rightSharedIndex = 256*(rightThreadIndex+1)-1;
		s_data[rightSharedIndex] = BINARY_OP(s_data[rightSharedIndex],s_data[leftSharedIndex]);
	}
#endif

#if X>=2
	memoryBarrierShared();
	if(tIndex < 2){
		uint leftSharedIndex  = 512*(leftThreadIndex+1)-1;
		uint rightSharedIndex = 512*(rightThreadIndex+1)-1;
		s_data[rightSharedIndex] = BINARY_OP(s_data[rightSharedIndex],s_data[leftSharedIndex]);
	}
#endif

#if X>=1
	memoryBarrierShared();
	if(tIndex < 1){
		uint leftSharedIndex  = 1024*(leftThreadIndex+1)-1;
		uint rightSharedIndex = 1024*(rightThreadIndex+1)-1;
		s_data[rightSharedIndex] = BINARY_OP(s_data[rightSharedIndex],s_data[leftSharedIndex]);
	}
#endif

	// clear last element, s.t. after down-sweep s_data[0] is BINARY_OP_NEUTRAL_ELEMENT
	//
	// note: no barrier needed since above loop only works on tIndex==0
	// in last iteration
	if(tIndex==0) {
#ifdef OUTPUT2
		AT(OUTPUT2,OUTPUT2_VAR,gl_WorkGroupID.x) = s_data[X*2-1];
#endif
		s_data[X*2-1] = BINARY_OP_NEUTRAL_ELEMENT;
	}

	// down-sweep, reverse parallel red.
	// we start at head of tree (take 2 times more threads each iteration)
	// stride is at 2**maxdepth(tree) here!


#if X>=1
	if(tIndex < 1){
		memoryBarrierShared();
		//same indexing as above
		uint leftSharedIndex  = 1024*(leftThreadIndex+1)-1;
		uint rightSharedIndex = 1024*(rightThreadIndex+1)-1;
		//swap left <- right, right <- left+right
		UNARY_OP_RETURN_TYPE temp =s_data[leftSharedIndex];
		s_data[leftSharedIndex] = s_data[rightSharedIndex];
		s_data[rightSharedIndex] = BINARY_OP(
				s_data[rightSharedIndex],
				temp
				);

	}
#endif

#if X>=2
	memoryBarrierShared();
	if(tIndex < 2){

		//same indexing as above
		uint leftSharedIndex  = 512*(leftThreadIndex+1)-1;
		uint rightSharedIndex = 512*(rightThreadIndex+1)-1;
		//swap left <- right, right <- left+right
		UNARY_OP_RETURN_TYPE temp =s_data[leftSharedIndex];
		s_data[leftSharedIndex] = s_data[rightSharedIndex];
		s_data[rightSharedIndex] = BINARY_OP(
				s_data[rightSharedIndex],
				temp
				);

	}
#endif

#if X>=4
	memoryBarrierShared();
	if(tIndex < 4){

		//same indexing as above
		uint leftSharedIndex  = 256*(leftThreadIndex+1)-1;
		uint rightSharedIndex = 256*(rightThreadIndex+1)-1;
		//swap left <- right, right <- left+right
		UNARY_OP_RETURN_TYPE temp =s_data[leftSharedIndex];
		s_data[leftSharedIndex] = s_data[rightSharedIndex];
		s_data[rightSharedIndex] = BINARY_OP(
				s_data[rightSharedIndex],
				temp
				);

	}
#endif

#if X>=8
	memoryBarrierShared();
	if(tIndex < 8){

		//same indexing as above
		uint leftSharedIndex  = 128*(leftThreadIndex+1)-1;
		uint rightSharedIndex = 128*(rightThreadIndex+1)-1;
		//swap left <- right, right <- left+right
		UNARY_OP_RETURN_TYPE temp =s_data[leftSharedIndex];
		s_data[leftSharedIndex] = s_data[rightSharedIndex];
		s_data[rightSharedIndex] = BINARY_OP(
				s_data[rightSharedIndex],
				temp
				);

	}
#endif
#if X>=16
	memoryBarrierShared();
	if(tIndex < 16){

		//same indexing as above
		uint leftSharedIndex  = 64*(leftThreadIndex+1)-1;
		uint rightSharedIndex = 64*(rightThreadIndex+1)-1;
		//swap left <- right, right <- left+right
		UNARY_OP_RETURN_TYPE temp =s_data[leftSharedIndex];
		s_data[leftSharedIndex] = s_data[rightSharedIndex];
		s_data[rightSharedIndex] = BINARY_OP(
				s_data[rightSharedIndex],
				temp
				);

	}
#endif

#if X>=32
	memoryBarrierShared();
	if(tIndex < 32){

		//same indexing as above
		uint leftSharedIndex  = 32*(leftThreadIndex+1)-1;
		uint rightSharedIndex = 32*(rightThreadIndex+1)-1;
		//swap left <- right, right <- left+right
		UNARY_OP_RETURN_TYPE temp =s_data[leftSharedIndex];
		s_data[leftSharedIndex] = s_data[rightSharedIndex];
		s_data[rightSharedIndex] = BINARY_OP(
				s_data[rightSharedIndex],
				temp
				);

	}
#endif
#if X>=64
	memoryBarrierShared();
	barrier();
	if(tIndex < 64){

		//same indexing as above
		uint leftSharedIndex  = 16*(leftThreadIndex+1)-1;
		uint rightSharedIndex = 16*(rightThreadIndex+1)-1;
		//swap left <- right, right <- left+right
		UNARY_OP_RETURN_TYPE temp =s_data[leftSharedIndex];
		s_data[leftSharedIndex] = s_data[rightSharedIndex];
		s_data[rightSharedIndex] = BINARY_OP(
				s_data[rightSharedIndex],
				temp
				);

	}
#endif
#if X>=128
	memoryBarrierShared();
	barrier();
	if(tIndex < 128){

		//same indexing as above
		uint leftSharedIndex  = 8*(leftThreadIndex+1)-1;
		uint rightSharedIndex = 8*(rightThreadIndex+1)-1;
		//swap left <- right, right <- left+right
		UNARY_OP_RETURN_TYPE temp =s_data[leftSharedIndex];
		s_data[leftSharedIndex] = s_data[rightSharedIndex];
		s_data[rightSharedIndex] = BINARY_OP(
				s_data[rightSharedIndex],
				temp
				);

	}
#endif
#if X>=256
	memoryBarrierShared();
	barrier();
	if(tIndex < 256){

		//same indexing as above
		uint leftSharedIndex  = 4*(leftThreadIndex+1)-1;
		uint rightSharedIndex = 4*(rightThreadIndex+1)-1;
		//swap left <- right, right <- left+right
		UNARY_OP_RETURN_TYPE temp =s_data[leftSharedIndex];
		s_data[leftSharedIndex] = s_data[rightSharedIndex];
		s_data[rightSharedIndex] = BINARY_OP(
				s_data[rightSharedIndex],
				temp
				);

	}
#endif
#if X>=512
	memoryBarrierShared();
	barrier();
	if(tIndex < 512){

		//same indexing as above
		uint leftSharedIndex  = 2*(leftThreadIndex+1)-1;
		uint rightSharedIndex = 2*(rightThreadIndex+1)-1;
		//swap left <- right, right <- left+right
		UNARY_OP_RETURN_TYPE temp =s_data[leftSharedIndex];
		s_data[leftSharedIndex] = s_data[rightSharedIndex];
		s_data[rightSharedIndex] = BINARY_OP(
				s_data[rightSharedIndex],
				temp
				);

	}
#endif
#if X>=1024
	memoryBarrierShared();
	barrier();
	if(tIndex < 1024){

		//same indexing as above
		uint leftSharedIndex  = 1*(leftThreadIndex+1)-1;
		uint rightSharedIndex = 1*(rightThreadIndex+1)-1;
		//swap left <- right, right <- left+right
		UNARY_OP_RETURN_TYPE temp =s_data[leftSharedIndex];
		s_data[leftSharedIndex] = s_data[rightSharedIndex];
		s_data[rightSharedIndex] = BINARY_OP(
				s_data[rightSharedIndex],
				temp
				);

	}
#endif

	memoryBarrierShared();
	barrier();
	// spread out partial scan by MULTIPLE_ELEMENTS stored in raking
	for(int j = 0; j < MULTIPLE_ELEMENTS; j++) {
		AT(OUTPUT,OUTPUT_VAR,globalIndexLeft+j) = s_data[leftThreadIndex]+leftRaking[j];
		AT(OUTPUT,OUTPUT_VAR,globalIndexRight+j) = s_data[rightThreadIndex]+rightRaking[j];
	}

}

