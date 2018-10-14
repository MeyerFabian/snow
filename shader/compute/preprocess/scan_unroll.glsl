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

  // raking sequantial global loads all values in raking get stored in registers for global writes at end
  uint[MULTIPLE_ELEMENTS] leftRaking;
  uint[MULTIPLE_ELEMENTS] rightRaking;

  if(globalIndexLeft > bufferSize) return;
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

  uint leftSharedIndex =0;
  uint rightSharedIndex = 0;
  int stride =1;
  for(int pow_d_of_2 = X; pow_d_of_2 > 0; pow_d_of_2 >>= 1) {
    memoryBarrierShared();
    barrier();

    //skip last(!) X-2**d threads
    //( => first run all threads, last run only tIndex==0)
    if (tIndex < pow_d_of_2) {
      // stride == 2**(d-max_depth(tree))
      // if leftSharedIndex is 2**(i-1)-1
      // rightSharedIndex is 2**i-1
      leftSharedIndex  = stride*(leftThreadIndex+1)-1;
      rightSharedIndex = stride*(rightThreadIndex+1)-1;
      // e.g. stride=8, tIndex==0 => s_data[7] + s_data[15]
      s_data[rightSharedIndex] = BINARY_OP(s_data[rightSharedIndex],s_data[leftSharedIndex]);
    }
    stride *= 2;
  }


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
  for(int d=1;d < 2*X ;d*=2){
    stride>>=1;
    memoryBarrierShared();
    barrier();
    if(tIndex < d){
      //same indexing as above
      uint leftSharedIndex  = stride*(leftThreadIndex+1)-1;
      uint rightSharedIndex = stride*(rightThreadIndex+1)-1;
      //swap left <- right, right <- left+right
      UNARY_OP_RETURN_TYPE temp =s_data[leftSharedIndex];
      s_data[leftSharedIndex] = s_data[rightSharedIndex];
      s_data[rightSharedIndex] = BINARY_OP(
	  s_data[rightSharedIndex],
	  temp
	  );

    }
  }
  memoryBarrierShared();
  barrier();
  // spread out partial scan by MULTIPLE_ELEMENTS stored in raking
  for(int j = 0; j < MULTIPLE_ELEMENTS; j++) {
    AT(OUTPUT,OUTPUT_VAR,globalIndexLeft+j) = s_data[leftThreadIndex]+leftRaking[j];
    AT(OUTPUT,OUTPUT_VAR,globalIndexRight+j) = s_data[rightThreadIndex]+rightRaking[j];
  }

}

