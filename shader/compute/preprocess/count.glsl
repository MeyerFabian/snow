#version 440
#include "shader/shared_hpp/voxel_tile_size.hpp"
#include "shader/compute/indexing/gridIndex.include.glsl"
layout(local_size_x =X, local_size_y =Y,local_size_z =Z)in;

uniform uvec3 gGridDim;
uniform PREC_VEC3_TYPE gGridPos;
uniform PREC_SCAL_TYPE gridSpacing;

uniform uint bufferSize;

void main(void){
  uint parIndex = gl_GlobalInvocationID.x;
  if(parIndex>=bufferSize){
    return;
  }
  PREC_VEC3_TYPE pos = AT(INPUT,INPUT_VAR,parIndex).xyz;

  PREC_VEC3_TYPE ParticleInGrid= (pos-gGridPos)/gridSpacing;
  ivec3 globalGridIndex = ivec3(ParticleInGrid);
  if(inBounds(globalGridIndex,gGridDim)){
    uint voxelAndTileIndex = get_voxel_and_tile_index(globalGridIndex,gGridDim);
    atomicAdd(AT(OUTPUT,OUTPUT_VAR,voxelAndTileIndex),1);
  }
}
