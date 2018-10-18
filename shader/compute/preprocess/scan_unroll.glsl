
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
 
#ifndef INPUT_INDEX_BUFFER
#define INPUT_INDEX_BUFFER 0
#endif

#ifndef OUTPUT_INDEX_BUFFER
#define OUTPUT_INDEX_BUFFER 0
#endif

#ifndef OUTPUT2_INDEX_BUFFER
#define OUTPUT2_INDEX_BUFFER 0
#endif

#define LOG_NUM_BANKS 5

// else case generally faster
#ifdef ZERO_BANK_CONFLICTS
#define CONFLICT_FREE_OFFSET(n) \
  (((n) >> LOG_NUM_BANKS) + ((n) >> (2 * LOG_NUM_BANKS)))
#else
#define CONFLICT_FREE_OFFSET(n) ((n) >> LOG_NUM_BANKS)
#endif

shared UNARY_OP_RETURN_TYPE s_data[X*2 + CONFLICT_FREE_OFFSET(X*2-1)];

// i will prob only use +

uniform uint bufferSize;

void main(void){
  uint tIndex = gl_LocalInvocationIndex;
  uint globalIndexLeft = MULTIPLE_ELEMENTS*(gl_WorkGroupID.x * X * 2 + tIndex);
  uint globalIndexRight = MULTIPLE_ELEMENTS*(gl_WorkGroupID.x * X * 2 + tIndex + X);

  if(globalIndexLeft > bufferSize) return;
  // raking sequantial global loads all values in raking get stored in registers for global writes at end

  uint[MULTIPLE_ELEMENTS-1] leftRaking;
  uint[MULTIPLE_ELEMENTS-1] rightRaking;

	leftRaking[0]  = UNARY_OP(INPUT_AT(INPUT,INPUT_VAR,INPUT_SIZE,globalIndexLeft ,INPUT_NUM_BUFFER,INPUT_INDEX_BUFFER));
	rightRaking[0] = UNARY_OP(INPUT_AT(INPUT,INPUT_VAR,INPUT_SIZE,globalIndexRight,INPUT_NUM_BUFFER,INPUT_INDEX_BUFFER));
	for(int j = 0; j < MULTIPLE_ELEMENTS-2; j++) {
		leftRaking[j+1]  = BINARY_OP(
				leftRaking[j],
				UNARY_OP(INPUT_AT(INPUT,INPUT_VAR,INPUT_SIZE,globalIndexLeft +j+1,INPUT_NUM_BUFFER,INPUT_INDEX_BUFFER))
				);

		rightRaking[j+1] = BINARY_OP(
				rightRaking[j],
				UNARY_OP(INPUT_AT(INPUT,INPUT_VAR,INPUT_SIZE,globalIndexRight+j+1,INPUT_NUM_BUFFER,INPUT_INDEX_BUFFER))
				);
	}
	// put partial reduced result in shared data
	s_data[tIndex +  CONFLICT_FREE_OFFSET(tIndex)] = BINARY_OP(
			leftRaking[MULTIPLE_ELEMENTS-2],
			UNARY_OP(INPUT_AT(INPUT,INPUT_VAR,INPUT_SIZE,globalIndexLeft + MULTIPLE_ELEMENTS - 1,INPUT_NUM_BUFFER,INPUT_INDEX_BUFFER))
			);
	s_data[tIndex + X + CONFLICT_FREE_OFFSET(tIndex + X) ] = BINARY_OP(
			rightRaking[MULTIPLE_ELEMENTS-2],
			UNARY_OP(INPUT_AT(INPUT,INPUT_VAR,INPUT_SIZE,globalIndexRight + MULTIPLE_ELEMENTS - 1,INPUT_NUM_BUFFER,INPUT_INDEX_BUFFER))
			);

  //interleaved parallel reduction with reversed indices
  //tree up-sweep (we start at leaves, d= max_depth(tree))
  //
  //
  int stride = 1;
#if X>=1024
  memoryBarrierShared();
  barrier();
  if(tIndex < 1024){
    uint left  = stride*2*tIndex + stride - 1;
    uint right = left + stride;
    left += CONFLICT_FREE_OFFSET(left);
    right += CONFLICT_FREE_OFFSET(right);
    s_data[right] =BINARY_OP(s_data[right],s_data[left]);
  }
  stride*=2;
#endif

#if X>=512
  memoryBarrierShared();
  barrier();
  if(tIndex < 512){
    uint left  = stride*2*tIndex + stride - 1;
    uint right = left + stride;
    left += CONFLICT_FREE_OFFSET(left);
    right += CONFLICT_FREE_OFFSET(right);
    s_data[right] =BINARY_OP(s_data[right],s_data[left]);
  }
  stride*=2;
#endif


#if X>=256
  memoryBarrierShared();
  barrier();
  if(tIndex < 256){
    uint left  = stride*2*tIndex + stride - 1;
    uint right = left + stride;
    left += CONFLICT_FREE_OFFSET(left);
    right += CONFLICT_FREE_OFFSET(right);
    s_data[right] =BINARY_OP(s_data[right],s_data[left]);
  }
  stride*=2;
#endif

#if X>=128
  memoryBarrierShared();
  barrier();
  if(tIndex < 128){
    uint left  = stride*2*tIndex + stride - 1;
    uint right = left + stride;
    left += CONFLICT_FREE_OFFSET(left);
    right += CONFLICT_FREE_OFFSET(right);
    s_data[right] =BINARY_OP(s_data[right],s_data[left]);
  }
  stride*=2;
#endif

#if X>=64
  memoryBarrierShared();
  barrier();
  if(tIndex < 64){
    uint left  = stride*2*tIndex + stride - 1;
    uint right = left + stride;
    left += CONFLICT_FREE_OFFSET(left);
    right += CONFLICT_FREE_OFFSET(right);
    s_data[right] =BINARY_OP(s_data[right],s_data[left]);
  }
  stride*=2;
#endif

#if X>=32
  memoryBarrierShared();
  barrier();
  if(tIndex < 32){
    uint left  = stride*2*tIndex + stride - 1;
    uint right = left + stride;
    left += CONFLICT_FREE_OFFSET(left);
    right += CONFLICT_FREE_OFFSET(right);
    s_data[right] =BINARY_OP(s_data[right],s_data[left]);
  }
  stride*=2;
#endif

#if X>=16
  memoryBarrierShared();
  if(tIndex < 16){
    uint left  = stride*2*tIndex + stride - 1;
    uint right = left + stride;
    left += CONFLICT_FREE_OFFSET(left);
    right += CONFLICT_FREE_OFFSET(right);
    s_data[right] =BINARY_OP(s_data[right],s_data[left]);
  }
  stride*=2;
#endif

#if X>=8
  memoryBarrierShared();
  if(tIndex < 8){
    uint left  = stride*2*tIndex + stride - 1;
    uint right = left + stride;
    left += CONFLICT_FREE_OFFSET(left);
    right += CONFLICT_FREE_OFFSET(right);
    s_data[right] =BINARY_OP(s_data[right],s_data[left]);
  }
  stride*=2;
#endif


#if X>=4
  memoryBarrierShared();
  if(tIndex < 4){
    uint left  = stride*2*tIndex + stride - 1;
    uint right = left + stride;
    left += CONFLICT_FREE_OFFSET(left);
    right += CONFLICT_FREE_OFFSET(right);
    s_data[right] =BINARY_OP(s_data[right],s_data[left]);
  }

  stride*=2;
#endif

#if X>=2
  memoryBarrierShared();
  if(tIndex < 2){
    uint left  = stride*2*tIndex + stride - 1;
    uint right = left + stride;
    left += CONFLICT_FREE_OFFSET(left);
    right += CONFLICT_FREE_OFFSET(right);
    s_data[right] =BINARY_OP(s_data[right],s_data[left]);
  }

  stride*=2;
#endif

#if X>=1
  memoryBarrierShared();
  if(tIndex < 1){
    uint left  = stride*2*tIndex + stride - 1;
    uint right = left + stride;
    left += CONFLICT_FREE_OFFSET(left);
    right += CONFLICT_FREE_OFFSET(right);
    s_data[right] =BINARY_OP(s_data[right],s_data[left]);
  }

#endif

  // clear last element, s.t. after down-sweep s_data[0] is BINARY_OP_NEUTRAL_ELEMENT
  //
  // note: no barrier needed since above loop only works on tIndex==0
  // in last iteration
  // down-sweep, reverse parallel red.
  if(tIndex==0) {
    uint last = X*2-1 + CONFLICT_FREE_OFFSET(X*2-1);
#ifdef OUTPUT2
	OUTPUT2_AT(OUTPUT2,OUTPUT2_VAR,OUTPUT2_SIZE,gl_WorkGroupID.x,OUTPUT2_NUM_BUFFER,OUTPUT2_INDEX_BUFFER) = s_data[last];
#endif
    s_data[last] = BINARY_OP_NEUTRAL_ELEMENT;
  }

  // we start at head of tree (take 2 times more threads each iteration)
  // stride is at 2**maxdepth(tree) here!


#if X>=1
  memoryBarrierShared();
  if(tIndex < 1){
    //same indexing as above
    uint left  = stride*2*tIndex + stride - 1;
    uint right = left + stride;
    left += CONFLICT_FREE_OFFSET(left);
    right += CONFLICT_FREE_OFFSET(right);

    //swap left <- right, right <- left+right
    UNARY_OP_RETURN_TYPE temp = s_data[left];
    s_data[left] = s_data[right];
    s_data[right] = BINARY_OP(s_data[right], temp);

  }
  stride>>=1;
#endif

#if X>=2
  memoryBarrierShared();
  if(tIndex < 2){

    uint left  = stride*2*tIndex + stride - 1;
    uint right = left + stride;
    left += CONFLICT_FREE_OFFSET(left);
    right += CONFLICT_FREE_OFFSET(right);

    //swap left <- right, right <- left+right
    UNARY_OP_RETURN_TYPE temp = s_data[left];
    s_data[left] = s_data[right];
    s_data[right] = BINARY_OP(s_data[right], temp);

  }

  stride>>=1;
#endif

#if X>=4
  memoryBarrierShared();
  if(tIndex < 4){
    uint left  = stride*2*tIndex + stride - 1;
    uint right = left + stride;
    left += CONFLICT_FREE_OFFSET(left);
    right += CONFLICT_FREE_OFFSET(right);

    //swap left <- right, right <- left+right
    UNARY_OP_RETURN_TYPE temp = s_data[left];
    s_data[left] = s_data[right];
    s_data[right] = BINARY_OP(s_data[right], temp);

  }

  stride>>=1;
#endif

#if X>=8
  memoryBarrierShared();
  if(tIndex < 8){

    uint left  = stride*2*tIndex + stride - 1;
    uint right = left + stride;
    left += CONFLICT_FREE_OFFSET(left);
    right += CONFLICT_FREE_OFFSET(right);

    //swap left <- right, right <- left+right
    UNARY_OP_RETURN_TYPE temp = s_data[left];
    s_data[left] = s_data[right];
    s_data[right] = BINARY_OP(s_data[right], temp);

  }

  stride>>=1;
#endif
#if X>=16
  memoryBarrierShared();
  if(tIndex < 16){
    uint left  = stride*2*tIndex + stride - 1;
    uint right = left + stride;
    left += CONFLICT_FREE_OFFSET(left);
    right += CONFLICT_FREE_OFFSET(right);

    //swap left <- right, right <- left+right
    UNARY_OP_RETURN_TYPE temp = s_data[left];
    s_data[left] = s_data[right];
    s_data[right] = BINARY_OP(s_data[right], temp);

  }

  stride>>=1;
#endif

#if X>=32
  memoryBarrierShared();
  if(tIndex < 32){
    uint left  = stride*2*tIndex + stride - 1;
    uint right = left + stride;
    left += CONFLICT_FREE_OFFSET(left);
    right += CONFLICT_FREE_OFFSET(right);

    //swap left <- right, right <- left+right
    UNARY_OP_RETURN_TYPE temp = s_data[left];
    s_data[left] = s_data[right];
    s_data[right] = BINARY_OP(s_data[right], temp);

  }

  stride>>=1;
#endif
#if X>=64
  memoryBarrierShared();
  barrier();
  if(tIndex < 64){
    uint left  = stride*2*tIndex + stride - 1;
    uint right = left + stride;
    left += CONFLICT_FREE_OFFSET(left);
    right += CONFLICT_FREE_OFFSET(right);

    //swap left <- right, right <- left+right
    UNARY_OP_RETURN_TYPE temp = s_data[left];
    s_data[left] = s_data[right];
    s_data[right] = BINARY_OP(s_data[right], temp);

  }
  stride>>=1;
#endif
#if X>=128
  memoryBarrierShared();
  barrier();
  if(tIndex < 128){
    uint left  = stride*2*tIndex + stride - 1;
    uint right = left + stride;
    left += CONFLICT_FREE_OFFSET(left);
    right += CONFLICT_FREE_OFFSET(right);

    //swap left <- right, right <- left+right
    UNARY_OP_RETURN_TYPE temp = s_data[left];
    s_data[left] = s_data[right];
    s_data[right] = BINARY_OP(s_data[right], temp);

  }
  stride>>=1;
#endif
#if X>=256
  memoryBarrierShared();
  barrier();
  if(tIndex < 256){
    uint left  = stride*2*tIndex + stride - 1;
    uint right = left + stride;
    left += CONFLICT_FREE_OFFSET(left);
    right += CONFLICT_FREE_OFFSET(right);

    //swap left <- right, right <- left+right
    UNARY_OP_RETURN_TYPE temp = s_data[left];
    s_data[left] = s_data[right];
    s_data[right] = BINARY_OP(s_data[right], temp);

  }

  stride>>=1;
#endif
#if X>=512
  memoryBarrierShared();
  barrier();
  if(tIndex < 512){
    uint left  = stride*2*tIndex + stride - 1;
    uint right = left + stride;
    left += CONFLICT_FREE_OFFSET(left);
    right += CONFLICT_FREE_OFFSET(right);

    //swap left <- right, right <- left+right
    UNARY_OP_RETURN_TYPE temp = s_data[left];
    s_data[left] = s_data[right];
    s_data[right] = BINARY_OP(s_data[right], temp);

  }

  stride>>=1;
#endif
#if X>=1024
  memoryBarrierShared();
  barrier();
  if(tIndex < 1024){
    uint left  = stride*2*tIndex + stride - 1;
    uint right = left + stride;
    left += CONFLICT_FREE_OFFSET(left);
    right += CONFLICT_FREE_OFFSET(right);

    //swap left <- right, right <- left+right
    UNARY_OP_RETURN_TYPE temp = s_data[left];
    s_data[left] = s_data[right];
    s_data[right] = BINARY_OP(s_data[right], temp);

  }
#endif

  memoryBarrierShared();
  barrier();

	OUTPUT_AT(OUTPUT,OUTPUT_VAR,OUTPUT_SIZE,globalIndexLeft,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER) = s_data[tIndex +CONFLICT_FREE_OFFSET(tIndex)];
	OUTPUT_AT(OUTPUT,OUTPUT_VAR,OUTPUT_SIZE,globalIndexRight,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER) = s_data[tIndex+X +CONFLICT_FREE_OFFSET(tIndex+X)];
	// spread out partial scan by MULTIPLE_ELEMENTS stored in raking
	for(int j = 0; j < MULTIPLE_ELEMENTS-1; j++) {
		OUTPUT_AT(OUTPUT,OUTPUT_VAR,OUTPUT_SIZE,globalIndexLeft+j+1,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER) = s_data[tIndex +CONFLICT_FREE_OFFSET(tIndex)]+leftRaking[j];
		OUTPUT_AT(OUTPUT,OUTPUT_VAR,OUTPUT_SIZE,globalIndexRight+j+1,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER) = s_data[tIndex+X +CONFLICT_FREE_OFFSET(tIndex+X)]+rightRaking[j];
	}

}

