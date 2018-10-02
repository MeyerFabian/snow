#version 440

#extension GL_ARB_gpu_shader_fp64 : enable

layout(local_size_x =X)in;

/*
 * Macros to be defined:
 *
 * INPUT(id) in_buffer[id]
 * OUTPUT(id) out_buffer[id]
 * where buffer needs to be included
 *
 * UNARY_OP(value) e.g. func(value)
 * UNARY_OP_RETURN_TYPE e.g. float
 * BINARY_OP(left,right) e.g. left+right
 */

shared UNARY_OP_RETURN_TYPE s_data[gl_WorkGroupSize.x];


void main(void){
  uint b_id = gl_WorkGroupID.x;
  uint b_size = gl_WorkGroupSize.x;
  uint t_id = gl_LocalInvocationIndex;
  uint g_id = gl_GlobalInvocationID.x;

  s_data[t_id] = UNARY_OP(AT(INPUT,INPUT_VAR,g_id));

  memoryBarrierShared();
  barrier();
  for(uint s=b_size/2; s > 0; s >>= 1) {
    if (t_id < s) {
      s_data[t_id] = BINARY_OP(s_data[t_id],s_data[t_id + s]);
    }
    memoryBarrierShared();
    barrier();
  }

  if(t_id ==0) AT(OUTPUT,OUTPUT_VAR,gl_WorkGroupID.x) = s_data[0];
}
