#version 440

#extension GL_ARB_gpu_shader_fp64 : enable

layout(local_size_x =X)in;

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

#ifndef INPUT_INDEX_BUFFER
#define INPUT_INDEX_BUFFER 0
#endif

#ifndef OUTPUT_INDEX_BUFFER
#define OUTPUT_INDEX_BUFFER 0
#endif


shared UNARY_OP_RETURN_TYPE s_data[gl_WorkGroupSize.x];


void main(void){
  uint b_id = gl_WorkGroupID.x;
  uint b_size = gl_WorkGroupSize.x;
  uint t_id = gl_LocalInvocationIndex;
  uint g_id = gl_GlobalInvocationID.x;

  s_data[t_id] = UNARY_OP(INPUT_AT(INPUT,INPUT_VAR,INPUT_SIZE,g_id,INPUT_NUM_BUFFER,INPUT_INDEX_BUFFER));

  memoryBarrierShared();
  barrier();
  for(uint s=b_size/2; s > 0; s >>= 1) {
    if (t_id < s) {
      s_data[t_id] = BINARY_OP(s_data[t_id],s_data[t_id + s]);
    }
    memoryBarrierShared();
    barrier();
  }

  if(t_id ==0) OUTPUT_AT(OUTPUT,OUTPUT_VAR,OUTPUT_SIZE,gl_WorkGroupID.x,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER) = s_data[0];
}
