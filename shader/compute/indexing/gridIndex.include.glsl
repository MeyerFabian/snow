#ifndef GRIDINDEX_GLSL
#define GRIDINDEX_GLSL

#include "shader/shared_hpp/voxel_block_size.hpp"

/**
 * Takes an integer vector ijk and returns the respective linearized index.
 * i of [0,x-dim]
 * j of[0,y-dim]
 * k of [0, z-dim]
 */
uint get_dim_index(const uvec3 ijk,const uvec3 dim){
	return ijk.x + (ijk.y * dim.x) + (ijk.z *dim.y * dim.x);
}

bool inBounds(const ivec3 index, const uvec3 dim){
	return index.x>= 0 && index.y>=0 && index.z>=0 && index.x < int(dim.x) && index.y < int(dim.y) && index.z< int(dim.z);
}
uint get_block_index(const uvec3 globalID, const uvec3
		globalDim){

	uvec3 blockID = uvec3(
			globalID.x>>VOXEL_SIZE_X_BIT,
			globalID.y>>VOXEL_SIZE_Y_BIT,
			globalID.z>>VOXEL_SIZE_Z_BIT);

	uvec3 blockDim = uvec3(
			globalDim.x>>VOXEL_SIZE_X_BIT,
			globalDim.y>>VOXEL_SIZE_Y_BIT,
			globalDim.z>>VOXEL_SIZE_Z_BIT);

	return get_dim_index(blockID,blockDim);
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
	return get_dim_index(voxelID,voxelDim);
}

uint get_voxel_and_block_index(const uvec3 globalID, const uvec3 globalDim){
	uint blockIndex = get_block_index(globalID, globalDim);

	uint voxelIndex = get_voxel_index(globalID);

	uint voxelDim_flat =VOXEL_DIM_X*VOXEL_DIM_Y*VOXEL_DIM_Z;

	return blockIndex *voxelDim_flat + voxelIndex;
}
uint get_voxel_and_block_index(const uint voxelIndex,const uint blockIndex){

	uint voxelDim_flat =VOXEL_DIM_X*VOXEL_DIM_Y*VOXEL_DIM_Z;

	return blockIndex *voxelDim_flat + voxelIndex;
}

uvec3 getBlockID(const uint blockIndex,const uvec3 globalDim){

	uvec3 blockDim = uvec3(
			globalDim.x>>VOXEL_SIZE_X_BIT,
			globalDim.y>>VOXEL_SIZE_Y_BIT,
			globalDim.z>>VOXEL_SIZE_Z_BIT);

	return getIJK(blockIndex,blockDim);

}

/*
 *
 * Convert from per block index to a global grid indexing
 * in:
 * @globalIndex : global thread index             : 4
 * @globalDim   : 3D dimensions of grid in voxels : (128,128,128)
 * @return      : global grid index
 *
 * First block xy-slice:
 *
 *  0  1  2  3       0   1   2   3
 *  4  5  6  7  -> 128 129 130 131
 *  8  9 10 11     256 257 258 259
 * 12 13 14 15     384 385 386 389
 */
uint global_indexing_of_voxel_and_block(const uint globalIndex, const uvec3 globalDim){

	// 3D dimensions of grid in blocks
	// example: (128,128,128)/(4,4,4)         : (32,32,32)
	uvec3 blockDim = uvec3(
			globalDim.x>>VOXEL_SIZE_X_BIT,
			globalDim.y>>VOXEL_SIZE_Y_BIT,
			globalDim.z>>VOXEL_SIZE_Z_BIT);


	// block indexing, example: 8/64          : 0
	uint blockIndex = globalIndex>>(VOXEL_SIZE_X_BIT+VOXEL_SIZE_Y_BIT+VOXEL_SIZE_Z_BIT);
	// example                                : (0,0,0)
	uvec3 blockID = getIJK(blockIndex,blockDim);

	uvec3 voxelDim = uvec3(
			VOXEL_DIM_X,
			VOXEL_DIM_Y,
			VOXEL_DIM_Z);

	// voxel index within block
	// example: 4%64                          : 4
	uint voxelIndexInBlock = globalIndex % (VOXEL_DIM_X*VOXEL_DIM_Y*VOXEL_DIM_Z);
	// example                                : (0,1,0)
	uvec3 voxelIDInBlock = getIJK(voxelIndexInBlock,voxelDim);


	// get_dim_index((0,1,0),(128,128,128))   : 128
	return get_dim_index(
			voxelIDInBlock +
			voxelDim * blockID
			,globalDim);
}
#endif
