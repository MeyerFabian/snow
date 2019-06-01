#version 440
#extension GL_NV_shader_atomic_float: enable

uniform uint indexSize;

#include "shader/compute/interpolation/cubic.include.glsl"

/*
 * Shader does a 3D-Interpolation from Particles to Grid. Sorting is important for caching and coalescing.
 * OUTPUT_BLOCK is result of stream compaction of active blocks (who have particles in them).
 * A push shader pushes the interpolated data from a particle onto a grid node in its support.
 * Each grid node is part of a block s.t. a particle that is part of this block can push its
 * information onto a grid node within the block or a neighboring block (in the halo).
 * The halo gets temporarily stored in shared memory, then an atomicadd into global memory is done.
 *
 * SortedBufferDataAccess ->
 * @INPUT              : Particle quantity (Particle_pos_vol,Particle_vel_mass)
 * @INPUT_COUNT        : Per cell particle counter
 * @INPUT_SCAN         : Scan over per cell particle counter
 *
 * BlockBufferData ->
 * @OUTPUT             : Grid quantity (Gridpoint_vel_mass)
 *
 * or
 *
 * BlockBufferDataAccess ->
 */

layout(local_size_x =X, local_size_y =Y,local_size_z =Z)in;

/* HALO is interpolation support left+right and voxel dimensions of the block
 * e.g.
 *   Halo = 6
 *      |
 * -----------
 * O O O O O O |-- Right support = 1
 * O O X X X O |
 * O O X X X O |-- Voxel Dimension = 3 = blockSize
 * O O X X X O |
 * O O O O O O
 * O O O O O O
 * ---
 *  |
 * Left support = 2
 */

#define HALO_X (VOXEL_DIM_X+LEFT_SUPPORT+RIGHT_SUPPORT)
#define HALO_Y (VOXEL_DIM_Y+LEFT_SUPPORT+RIGHT_SUPPORT)
#define HALO_Z (VOXEL_DIM_Z+LEFT_SUPPORT+RIGHT_SUPPORT)
// linearized shared buffer
shared PREC_VEC_TYPE temp [HALO_X*HALO_Y*HALO_Z];

#define blockSize uvec3(VOXEL_DIM_X,VOXEL_DIM_Y,VOXEL_DIM_Z)
void main(void){
#ifdef OUTPUT_BLOCK_INDEX
	// active block index
	uvec3 blockID = getBlockID(OUTPUT_BLOCK_INDEX_AT(OUTPUT_BLOCK_INDEX,OUTPUT_BLOCK_INDEX_VAR,OUTPUT_BLOCK_INDEX_SIZE,gl_WorkGroupID.x,OUTPUT_BLOCK_INDEX_NUM_BUFFER,OUTPUT_BLOCK_INDEX_INDEX_BUFFER), grid_def.gGridDim);
#else
	// get 3D representation of the block
	uvec3 blockID = getBlockID(gl_WorkGroupID.x,grid_def.gGridDim);
#endif
	// get 3D representation of voxel within block
	uvec3 t_ijk = getIJK(gl_LocalInvocationID.x,blockSize);
	// get 3D representation of voxel within grid
	uvec3 ijk = blockID * blockSize + t_ijk;

	// get the 1D offset in the grid buffer
	uint grid_key = SORTING_KEY(ijk,grid_def.gGridDim);
	// how many particles this cell will push into the neighborhood
	uint count = INPUT_COUNT_AT(INPUT_COUNT,INPUT_COUNT_VAR,INPUT_COUNT_SIZE,grid_key,INPUT_COUNT_NUM_BUFFER,INPUT_COUNT_INDEX_BUFFER);

	// the scan is the offset of this cell into the sorted particle buffer
	uint scan = INPUT_SCAN_AT(INPUT_SCAN,INPUT_SCAN_VAR,INPUT_SCAN_SIZE,grid_key,INPUT_SCAN_NUM_BUFFER,INPUT_SCAN_INDEX_BUFFER);

	// Clear information in shared memory (including halo). Since there are less threads (within block) than in the halo, threads jump one block size to the next memory position.
	for(int local_i = int(gl_LocalInvocationID.x); local_i < (HALO_X*HALO_Y*HALO_Z);local_i += X*Y*Z) {
		temp[local_i] = PREC_VEC_TYPE(0.0);
	}

	memoryBarrierShared();
	barrier();

	// uniform flow control -> tile_count
	for(int particle_i = 0; particle_i < count; particle_i++){
		//  extract position and velocity out of particle buffer
		uint globalParticleIndex = scan+particle_i;
		PREC_VEC3_TYPE pos = INPUT_AT(INPUT,Particle_pos_vol,INPUT_SIZE,globalParticleIndex,INPUT_NUM_BUFFER,INPUT_INDEX_BUFFER).xyz;


		PREC_VEC_TYPE vp_mp =
			INPUT_AT(INPUT,Particle_vel_mass,INPUT_SIZE,globalParticleIndex,INPUT_NUM_BUFFER,INPUT_INDEX_BUFFER);

		// transfer position to coordinate frame of grid
		PREC_VEC3_TYPE positionInGrid= (pos-grid_def.gGridPos)/grid_def.gridSpacing;

		// iterate over support
		for(int x = -LEFT_SUPPORT; x<= RIGHT_SUPPORT ;x++){
			for(int y = -LEFT_SUPPORT; y<= RIGHT_SUPPORT ;y++){
				for(int z = -LEFT_SUPPORT; z <= RIGHT_SUPPORT ;z++){

					// 3d interpolation offset for cubic x,y,z element of [-1;2]
					ivec3 gridOffset = ivec3(x,y,z);

					// 3D ID of the cell/voxel to do interpolation within the grid
					uvec3 global_grid_index = uvec3(ivec3(ijk)+gridOffset);

					// distance from node to particle
					PREC_VEC3_TYPE gridDistanceToParticle =vec3(global_grid_index) -  positionInGrid ;
					// weight according to interpolation function
					PREC_SCAL_TYPE wip = .0f;
					weighting (gridDistanceToParticle,wip);

					//Taylor 1. term
					PREC_VEC_TYPE vi_mi = PREC_VEC_TYPE(vp_mp.xyz,1.0)*vp_mp.w*wip;

					// 1D index of the cell/voxel to do interpolation within this block
					uint local_i = get_dim_index(t_ijk + uvec3(gridOffset+LEFT_SUPPORT),uvec3(HALO_X,HALO_Y,HALO_Z));
					//Interpolate momentum weighted by mass and interp. function; atomics no uniform flow control needed
					atomicAdd(temp[local_i].x,vi_mi.x );
					atomicAdd(temp[local_i].y,vi_mi.y );
					atomicAdd(temp[local_i].z,vi_mi.z );
					atomicAdd(temp[local_i].w,vi_mi.w );

				}
			}
		}

	}
	memoryBarrierShared();
	barrier();

	// most lower-left-front grid ID of this block within the grid
	ivec3 grid_start_node = ivec3(blockID * blockSize) - LEFT_SUPPORT;

	// Since there are less threads(one block) than memory positions in the halo,
	// they need to iteratively handle more than 1 memory position.
	// This loop skips forward one block size and handle next memory position.
	for(int local_i = int(gl_LocalInvocationID.x); local_i < (HALO_X*HALO_Y*HALO_Z);local_i += X*Y*Z) {
		// Find node within halo
		uvec3 halo_ijk = uvec3(getIJK(local_i,ivec3(HALO_X,HALO_Y,HALO_Z)));
		// Find node within grid
		ivec3 to_process = grid_start_node + ivec3(halo_ijk);
		// Check if within grid bounds
		if(inBounds(to_process,grid_def.gGridDim)){
			// Get 1D grid buffer offset for that node
			uint write_key = SORTING_KEY(to_process,grid_def.gGridDim);
			// Atomically add the partial result from particles within this block to the global memory
			PREC_VEC_TYPE to_write = temp[local_i];

			atomicAdd(OUTPUT_AT(OUTPUT,Gridpoint_vel_mass,OUTPUT_SIZE,write_key,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER).x,
					to_write.x);

			atomicAdd(OUTPUT_AT(OUTPUT,Gridpoint_vel_mass,OUTPUT_SIZE,write_key,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER).y,
					to_write.y);

			atomicAdd(OUTPUT_AT(OUTPUT,Gridpoint_vel_mass,OUTPUT_SIZE,write_key,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER).z,
					to_write.z);

			atomicAdd(OUTPUT_AT(OUTPUT,Gridpoint_vel_mass,OUTPUT_SIZE,write_key,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER).w,
					to_write.w);
		}
	}
}
