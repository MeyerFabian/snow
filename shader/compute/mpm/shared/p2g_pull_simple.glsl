#version 440
#extension GL_NV_shader_atomic_float: enable

//reverse supports
#define P2G_PULL
#include "shader/compute/interpolation/cubic.include.glsl"

/*
 * Shader does a 3D-Interpolation from Particles to Grid. Sorting is important for caching and coalescing.
 * OUTPUT_BLOCK is result of stream compaction of active blocks (who have particles in the halo).
 * A pull shader pulls the interpolated data from a grid node to a particle in its support.
 * Each grid node is part of a block s.t. a particle that is part of this block can pull its
 * information from a grid node within the block or a neighboring block (in the halo).
 * The halo gets temporarily stored in shared memory. In a pull method the last global write back
 * does not need to be atomic. This block is the only one that writes to these positions.
 *
 * SortedBufferDataAccess ->
 * @INPUT              : Particle quantity (Particle_pos_vol,Particle_vel_mass)
 * @INPUT_COUNT        : Per cell particle counter
 * @INPUT_SCAN         : Scan over per cell particle counter
 *
 * BlockBufferDataAccess ->
 * @OUTPUT             : Grid quantity (Gridpoint_vel_mass)
 * @OUTPUT_BLOCK_INDEX : Indexing over active blocks
 * @OUTPUT_BLOCK_COUNT : Maximum particle counter in a HALO(!)
 *
 */

layout(local_size_x =X, local_size_y =Y,local_size_z =Z)in;


/* HALO is interpolation support left+right and voxel dimensions of the block.
 * PULL Methods have reversed support!
 * e.g.
 *   Halo = 6
 *      |
 * -----------
 * O O O O O O |-- Right support = 2
 * O O O O O O /
 * O X X X O O |
 * O X X X O O |-- Voxel Dimension = 3 = blockSize
 * O X X X O O |
 * O O O O O O
 * -
 * |
 * Left support = 1
 */
#define HALO_X (VOXEL_DIM_X+LEFT_SUPPORT+RIGHT_SUPPORT)
#define HALO_Y (VOXEL_DIM_Y+LEFT_SUPPORT+RIGHT_SUPPORT)
#define HALO_Z (VOXEL_DIM_Z+LEFT_SUPPORT+RIGHT_SUPPORT)

// Linearized shared buffer needs to pull four grid variables
// pos, vel, scan, count
#define temp_pos 0
#define temp_vel HALO_X*HALO_Y*HALO_Z
shared PREC_VEC_TYPE temp [2*HALO_X*HALO_Y*HALO_Z];

#define num_scan 0
#define num_count HALO_X*HALO_Y*HALO_Z
shared uint num [2*HALO_X*HALO_Y*HALO_Z];

#define blockSize uvec3(VOXEL_DIM_X,VOXEL_DIM_Y,VOXEL_DIM_Z)
void main(void){
	// active block index
	uint blockIndex = OUTPUT_BLOCK_INDEX_AT(OUTPUT_BLOCK_INDEX,OUTPUT_BLOCK_INDEX_VAR,OUTPUT_BLOCK_INDEX_SIZE,gl_WorkGroupID.x,OUTPUT_BLOCK_INDEX_NUM_BUFFER,OUTPUT_BLOCK_INDEX_INDEX_BUFFER);

	// get 3D representation of the block
	uvec3 blockID = getBlockID(blockIndex, grid_def.gGridDim);

	// get 3D representation of voxel within block
	uvec3 t_ijk = getIJK(gl_LocalInvocationID.x,blockSize);

	// get 3D representation of voxel within grid
	uvec3 ijk = blockID * blockSize + t_ijk;

	// most lower-left-front grid ID of this block within the grid
	ivec3 grid_start_node = ivec3(blockID * blockSize) - LEFT_SUPPORT;

	// A memory barrier has to be in uniform flow control however. So we need to iterate the exact same amount of times for each thread within a block. The maximum per cell counter within a block is the minimum count to handle all particles.
	uint tile_count = OUTPUT_BLOCK_COUNT_AT(OUTPUT_BLOCK_COUNT,OUTPUT_BLOCK_COUNT_VAR,OUTPUT_BLOCK_COUNT_SIZE,blockIndex,OUTPUT_BLOCK_COUNT_NUM_BUFFER,OUTPUT_BLOCK_COUNT_INDEX_BUFFER);

	// Load the cell num and count within the HALO. Since there are less threads (within a block) than in the halo, threads jump one block size to the next memory position.
	for(int local_i = int(gl_LocalInvocationID.x); local_i < (HALO_X*HALO_Y*HALO_Z);local_i += X*Y*Z) {
		// Find node within halo
		uvec3 halo_ijk = uvec3(getIJK(local_i,ivec3(HALO_X,HALO_Y,HALO_Z)));
		// Find node within grid
		ivec3 to_process = grid_start_node + ivec3(halo_ijk);
		// Check if within grid bounds
		if(inBounds(to_process,grid_def.gGridDim)){
			// Get 1D grid buffer offset for that node
			uint read_key = SORTING_KEY(to_process,grid_def.gGridDim);

			// load scan + count
			num[num_count + local_i] = INPUT_COUNT_AT(INPUT_COUNT,INPUT_COUNT_VAR,INPUT_COUNT_SIZE,read_key,INPUT_COUNT_NUM_BUFFER,INPUT_COUNT_INDEX_BUFFER);

			num[num_scan + local_i] = INPUT_SCAN_AT(INPUT_SCAN,INPUT_SCAN_VAR,INPUT_SCAN_SIZE,read_key,INPUT_SCAN_NUM_BUFFER,INPUT_SCAN_INDEX_BUFFER);
		}
		else{
			// otherwise set count to 0 so in pull transfer we know to skip
			num[num_count + local_i] = 0;
		}
	}

	memoryBarrierShared();
	barrier();

	PREC_VEC_TYPE vi_mi = PREC_VEC_TYPE(0.0);
	// uniform flow control
	for(int particle_count = 0; particle_count < tile_count; particle_count++){
		// load one particle pos and vel for each node in the halo
		for(int local_i = int(gl_LocalInvocationID.x); local_i < (HALO_X*HALO_Y*HALO_Z);local_i += X*Y*Z) {
			// skip if this node does not have particles
			if(particle_count < num[num_count+local_i]){
				//  extract position and velocity out of particle buffer
				uint globalParticleIndex = num[num_scan + local_i] + particle_count;


				PREC_VEC3_TYPE pos = INPUT_AT(INPUT,Particle_pos_vol,INPUT_SIZE,globalParticleIndex,INPUT_NUM_BUFFER,INPUT_INDEX_BUFFER).xyz;
				temp[temp_pos + local_i] = PREC_VEC_TYPE((pos-grid_def.gGridPos)/grid_def.gridSpacing,0.0);
				temp[temp_vel + local_i] = INPUT_AT(INPUT,Particle_vel_mass,INPUT_SIZE,globalParticleIndex,INPUT_NUM_BUFFER,INPUT_INDEX_BUFFER);
			}
		}

		memoryBarrierShared();
		barrier();

		// Iterate over support
		for(int x = -LEFT_SUPPORT; x<= RIGHT_SUPPORT ;x++){
			for(int y = -LEFT_SUPPORT; y<= RIGHT_SUPPORT ;y++){
				for(int z = -LEFT_SUPPORT; z <= RIGHT_SUPPORT ;z++){
					// 3d interpolation offset for cubic x,y,z element of [-1;2]
					ivec3 gridOffset = ivec3(x,y,z);

					// 1D index of the cell/voxel to do interpolation within this block
					uint local_i = get_dim_index(t_ijk + uvec3(gridOffset+LEFT_SUPPORT),uvec3(HALO_X,HALO_Y,HALO_Z));
					//check again that this cell has a particle on this count
					if(particle_count < num[num_count+local_i]){
						// distance from node to particle
						PREC_VEC3_TYPE gridDistanceToParticle = vec3(ijk) - temp[temp_pos + local_i].xyz ;
						// weight according to interpolation function
						PREC_SCAL_TYPE wip = .0f;
						weighting (gridDistanceToParticle,wip);

						// PIC Transfer
						PREC_VEC_TYPE vp_mp = temp[temp_vel+local_i];
						vi_mi += PREC_VEC_TYPE(vp_mp.xyz,1.0)*vp_mp.w*wip;
					}

				}
			}
		}

		memoryBarrierShared();
		barrier();
	}


	// pretty simple write back
	uint write_key = SORTING_KEY(ijk,grid_def.gGridDim);
	OUTPUT_AT(OUTPUT,Gridpoint_vel_mass,OUTPUT_SIZE,write_key,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER) =
		vi_mi;

}
