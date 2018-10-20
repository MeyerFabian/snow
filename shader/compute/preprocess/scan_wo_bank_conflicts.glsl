#version 440
layout(local_size_x =X)in;

/*
 * Macros to be defined:
 *
 * {INPUT,OUTPUT,OUTPUT2} buffer
 * {INPUT,OUTPUT,OUTPUT2}_VAR var
 * {INPUT,OUTPUT,OUTPUT2}_SIZE buffer
 * {INPUT,OUTPUT,OUTPUT2}_NUM_BUFFER double/multi buffer
 * {INPUT,OUTPUT,OUTPUT2}_INDEX_BUFFER which of the multi buffers
 *
 * where buffer needs to be included
 * e.g. AOS-Layout =>
 * AT(buffer,var,index) =>
 * buffer[index].var
 *
 * UNARY_OP(value) length(value)
 * UNARY_OP_RETURN_TYPE float
 * BINARY_OP(left,right) left*right
 * BINARY_OP_NEUTRAL_ELEMENT 1
 */

// start with half the dispatch size

#define LOG_NUM_BANKS 5
// else case generally faster
#define ZERO_BANK_CONFLICTS
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
  uint globalIndex = gl_WorkGroupID.x * X * 2 + tIndex;

  if(globalIndex > bufferSize) return;
  // global loads at 1) t_id and 2) t_id + X
  s_data[tIndex + CONFLICT_FREE_OFFSET(tIndex)] = UNARY_OP(INPUT_AT(INPUT,INPUT_VAR,INPUT_SIZE,globalIndex ,INPUT_NUM_BUFFER,INPUT_INDEX_BUFFER));
  s_data[(tIndex + X) + CONFLICT_FREE_OFFSET(tIndex+X)] = UNARY_OP(INPUT_AT(INPUT,INPUT_VAR,INPUT_SIZE,globalIndex+X ,INPUT_NUM_BUFFER,INPUT_INDEX_BUFFER));

  //interleaved parallel reduction with reversed indices
  //tree up-sweep (we start at leaves, d= max_depth(tree))
  int stride =1;
  for(int pow_d_of_2 = X; pow_d_of_2 > 0; pow_d_of_2 >>= 1) {
    memoryBarrierShared();
    barrier();

    //skip last(!) X-2**d threads
    //( => first run all threads, last run only tIndex==0)
    if (tIndex < pow_d_of_2) {
      // stride == 2**(d-max_depth(tree))
      // if left is 2**(i-1)-1
      // right is 2**i-1
      uint left  = stride*2*tIndex + stride - 1;
      uint right = left + stride;
      left += CONFLICT_FREE_OFFSET(left);
      right += CONFLICT_FREE_OFFSET(right);
      // e.g. stride=8, tIndex==0 => s_data[7] + s_data[15]
      s_data[right] =
	BINARY_OP(
	    s_data[right],
	    s_data[left]
	    );
    }
    stride *= 2;
  }


  // clear last element, s.t. after down-sweep s_data[0] is BINARY_OP_NEUTRAL_ELEMENT
  //
  // note: no barrier needed since above loop only works on tIndex==0
  // in last iteration
  if(tIndex==0) {
    uint last = X*2-1 + CONFLICT_FREE_OFFSET(X*2-1);
#ifdef OUTPUT2
	OUTPUT2_AT(OUTPUT2,OUTPUT2_VAR,OUTPUT2_SIZE,gl_WorkGroupID.x,OUTPUT2_NUM_BUFFER,OUTPUT2_INDEX_BUFFER) = s_data[last];
#endif
    s_data[last] = BINARY_OP_NEUTRAL_ELEMENT;
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

      uint left  = stride*2*tIndex + stride - 1;
      uint right = left + stride;
      left += CONFLICT_FREE_OFFSET(left);
      right += CONFLICT_FREE_OFFSET(right);

      //swap left <- right, right <- left+right
      UNARY_OP_RETURN_TYPE temp = s_data[left];
      s_data[left] = s_data[right];
      s_data[right] = BINARY_OP(
	  s_data[right] ,
	  temp
	  );

    }
  }
  memoryBarrierShared();
  barrier();


  // global writes at 1) t_id and 2) t_id + X
  OUTPUT_AT(OUTPUT,OUTPUT_VAR,OUTPUT_SIZE,globalIndex,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER) = s_data[tIndex +CONFLICT_FREE_OFFSET(tIndex)];
  
  OUTPUT_AT(OUTPUT,OUTPUT_VAR,OUTPUT_SIZE,globalIndex+X,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER) = s_data[(tIndex+ X)+CONFLICT_FREE_OFFSET(tIndex+X)];

}
