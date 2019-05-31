#version 440
#extension GL_NV_shader_atomic_float: enable

uniform uint indexSize;

#include "shader/compute/interpolation/cubic.include.glsl"
layout(local_size_x =X, local_size_y =Y,local_size_z =Z)in;

#define HALO_X (VOXEL_DIM_X+LEFT_SUPPORT+RIGHT_SUPPORT)
#define HALO_Y (VOXEL_DIM_Y+LEFT_SUPPORT+RIGHT_SUPPORT)
#define HALO_Z (VOXEL_DIM_Z+LEFT_SUPPORT+RIGHT_SUPPORT)
shared PREC_VEC_TYPE temp [HALO_X*HALO_Y*HALO_Z];

#define blockSize uvec3(VOXEL_DIM_X,VOXEL_DIM_Y,VOXEL_DIM_Z)
void main(void){
#ifdef OUTPUT_BLOCK_INDEX
	uvec3 blockID = getBlockID(OUTPUT_BLOCK_INDEX_AT(OUTPUT_BLOCK_INDEX,OUTPUT_BLOCK_INDEX_VAR,OUTPUT_BLOCK_INDEX_SIZE,gl_WorkGroupID.x,OUTPUT_BLOCK_INDEX_NUM_BUFFER,OUTPUT_BLOCK_INDEX_INDEX_BUFFER), grid_def.gGridDim);
#else
	uvec3 blockID = getBlockID(gl_WorkGroupID.x,grid_def.gGridDim);
#endif
	uvec3 t_ijk = getIJK(gl_LocalInvocationID.x,blockSize);
	uvec3 ijk = blockID * blockSize + t_ijk;

	uint grid_key = SORTING_KEY(ijk,grid_def.gGridDim);
	uint count = INPUT_COUNT_AT(INPUT_COUNT,INPUT_COUNT_VAR,INPUT_COUNT_SIZE,grid_key,INPUT_COUNT_NUM_BUFFER,INPUT_COUNT_INDEX_BUFFER);

	uint scan = INPUT_SCAN_AT(INPUT_SCAN,INPUT_SCAN_VAR,INPUT_SCAN_SIZE,grid_key,INPUT_SCAN_NUM_BUFFER,INPUT_SCAN_INDEX_BUFFER);

	for(int local_i = int(gl_LocalInvocationID.x); local_i < (HALO_X*HALO_Y*HALO_Z);local_i += X*Y*Z) {
		temp[local_i] = PREC_VEC_TYPE(0.0);
	}

	memoryBarrierShared();
	barrier();
	for(int x = -LEFT_SUPPORT; x<= RIGHT_SUPPORT ;x++){
		for(int y = -LEFT_SUPPORT; y<= RIGHT_SUPPORT ;y++){
			for(int z = -LEFT_SUPPORT; z <= RIGHT_SUPPORT ;z++){
				ivec3 gridOffset = ivec3(x,y,z);
				uvec3 global_grid_index = uvec3(ivec3(ijk)+gridOffset);
				PREC_VEC_TYPE vi_mi=PREC_VEC_TYPE(0.0);
				for(int particle_i = 0; particle_i < count; particle_i++){
					uint globalParticleIndex = scan+particle_i;
					PREC_VEC3_TYPE pos = INPUT_AT(INPUT,Particle_pos_vol,INPUT_SIZE,globalParticleIndex,INPUT_NUM_BUFFER,INPUT_INDEX_BUFFER).xyz;


					PREC_VEC_TYPE vp_mp =
						INPUT_AT(INPUT,Particle_vel_mass,INPUT_SIZE,globalParticleIndex,INPUT_NUM_BUFFER,INPUT_INDEX_BUFFER);

					PREC_VEC3_TYPE positionInGrid= (pos-grid_def.gGridPos)/grid_def.gridSpacing;

					PREC_VEC3_TYPE gridDistanceToParticle =vec3(global_grid_index) -  positionInGrid ;
					PREC_SCAL_TYPE wip = .0f;
					weighting (gridDistanceToParticle,wip);


					vi_mi += PREC_VEC_TYPE(vp_mp.xyz,1.0)*vp_mp.w*wip;
				}
				uint local_i = get_dim_index(t_ijk + uvec3(gridOffset+LEFT_SUPPORT),uvec3(HALO_X,HALO_Y,HALO_Z));
				atomicAdd(temp[local_i].x,vi_mi.x );
				atomicAdd(temp[local_i].y,vi_mi.y );
				atomicAdd(temp[local_i].z,vi_mi.z );
				atomicAdd(temp[local_i].w,vi_mi.w );

				memoryBarrierShared();
				barrier();

			}
		}

	}
	memoryBarrierShared();
	barrier();

	ivec3 grid_start_node = ivec3(blockID * blockSize) - LEFT_SUPPORT;

	for(int local_i = int(gl_LocalInvocationID.x); local_i < (HALO_X*HALO_Y*HALO_Z);local_i += X*Y*Z) {
		uvec3 halo_ijk = uvec3(getIJK(local_i,ivec3(HALO_X,HALO_Y,HALO_Z)));
		ivec3 to_process = grid_start_node + ivec3(halo_ijk);
		if(inBounds(to_process,grid_def.gGridDim)){
			uint write_key = SORTING_KEY(to_process,grid_def.gGridDim);
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
