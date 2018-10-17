/**
 * Takes an integer vector ijk and returns the respective buffer index.
 * i of [0,x-GridDimension]
 * j of[0,y-GridDimension]
 * k of [0, z-GridDimension]
 */
/*
   void getIndex(const ivec3 ijk,inout int index){
   index = ijk.x + (ijk.y * int(gGridDim[0].x)) + (ijk.z *int(gGridDim[1].x) * int(gGridDim[0].x));
   }
 */
/**
 * Takes an integer vector ijk and returns the respective buffer index.
 * i of [0,x-dim]
 * j of[0,y-dim]
 * k of [0, z-dim]
 */
uint getIndex(const uvec3 ijk, uvec3 dim){
  return ijk.x + (ijk.y * dim.x) + (ijk.z *dim.y * dim.x);
}

bool inBounds(const ivec3 index, const uvec3 dim){
  return index.x>= 0 && index.y>=0 && index.z>=0 && index.x < dim.x && index.y < dim.y && index.z< dim.z;
}
uint get_tile_index(const uvec3 globalID, uvec3
    globalDim){

  uvec3 tileID = uvec3(
      globalID.x>>VOXEL_SIZE_X_BIT,
      globalID.y>>VOXEL_SIZE_Y_BIT,
      globalID.z>>VOXEL_SIZE_Z_BIT);

  uvec3 tileDim = uvec3(
      globalDim.x>>VOXEL_SIZE_X_BIT,
      globalDim.y>>VOXEL_SIZE_Y_BIT,
      globalDim.z>>VOXEL_SIZE_Z_BIT);

  return getIndex(tileID,tileDim);
}
uint get_voxel_index(const uvec3 globalID){
  uvec3 voxelDim = uvec3(
      VOXEL_DIM_X,
      VOXEL_DIM_Y,
      VOXEL_DIM_Z);

  uvec3 voxelID = uvec3(
      globalID.x%VOXEL_DIM_X,
      globalID.y%VOXEL_DIM_Y,
      globalID.z%VOXEL_DIM_Z);
  return getIndex(voxelID,voxelDim);
}

uint get_voxel_and_tile_index(const uvec3 globalID, uvec3 globalDim){
  uint tileIndex = get_tile_index(globalID, globalDim);

  uint voxelIndex = get_voxel_index(globalID);

  uint voxelDim_flat =VOXEL_DIM_X*VOXEL_DIM_Y*VOXEL_DIM_Z;

  return tileIndex *voxelDim_flat + voxelIndex;
}
uint get_voxel_and_tile_index(uint voxelIndex, uint tileIndex){

  uint voxelDim_flat =VOXEL_DIM_X*VOXEL_DIM_Y*VOXEL_DIM_Z;

  return tileIndex *voxelDim_flat + voxelIndex;
}
