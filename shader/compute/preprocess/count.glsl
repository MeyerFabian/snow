#version 440
#include "shader/compute/indexing/voxelAndTilePointer.include.glsl"
layout(local_size_x =X, local_size_y =Y,local_size_z =Z)in;
uniform uint bufferSize;

void main(void){
  uint i= gl_GlobalInvocationID.x;
  if(i>=bufferSize){
    return;
  }
  atomicAdd(counters[i],1);
}
