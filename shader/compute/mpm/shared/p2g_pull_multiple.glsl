#version 440
#extension GL_NV_shader_atomic_float: enable

//reverse supports
#define P2G_PULL
#include "shader/compute/interpolation/cubic.include.glsl"

layout(local_size_x =X, local_size_y =Y,local_size_z =Z)in;

#define HALO_X (VOXEL_DIM_X+LEFT_SUPPORT+RIGHT_SUPPORT)
#define HALO_Y (VOXEL_DIM_Y+LEFT_SUPPORT+RIGHT_SUPPORT)
#define HALO_Z (VOXEL_DIM_Z+LEFT_SUPPORT+RIGHT_SUPPORT)
#define HALO_FLAT HALO_X*HALO_Y*HALO_Z

// somehow opengl either bugs out with two shared mem. variables of same type:
// i tried padding without results.
// this here is a workaround:
#define temp_pos 0
#define temp_vel HALO_FLAT*MULTIPLE_PARTICLES
shared PREC_VEC_TYPE temp [2*HALO_FLAT*MULTIPLE_PARTICLES];

#define num_scan 0
#define num_count HALO_FLAT
shared uint num [2*HALO_FLAT];

#define blockSize uvec3(VOXEL_DIM_X,VOXEL_DIM_Y,VOXEL_DIM_Z)
void main(void){
	uint blockIndex = OUTPUT_BLOCK_INDEX_AT(OUTPUT_BLOCK_INDEX,OUTPUT_BLOCK_INDEX_VAR,OUTPUT_BLOCK_INDEX_SIZE,gl_WorkGroupID.x,OUTPUT_BLOCK_INDEX_NUM_BUFFER,OUTPUT_BLOCK_INDEX_INDEX_BUFFER);

	uvec3 blockID = getBlockID(blockIndex, grid_def.gGridDim);

	uvec3 t_ijk = getIJK(gl_LocalInvocationID.x,blockSize);
	uvec3 ijk = blockID * blockSize + t_ijk;
	ivec3 grid_start_node = ivec3(blockID * blockSize) - LEFT_SUPPORT;

	uint tile_count = OUTPUT_BLOCK_COUNT_AT(OUTPUT_BLOCK_COUNT,OUTPUT_BLOCK_COUNT_VAR,OUTPUT_BLOCK_COUNT_SIZE,blockIndex,OUTPUT_BLOCK_COUNT_NUM_BUFFER,OUTPUT_BLOCK_COUNT_INDEX_BUFFER);

	for(int local_i = int(gl_LocalInvocationID.x); local_i < HALO_FLAT;local_i += X*Y*Z) {

		uvec3 halo_ijk = uvec3(getIJK(local_i,ivec3(HALO_X,HALO_Y,HALO_Z)));
		ivec3 to_process = grid_start_node + ivec3(halo_ijk);
		if(inBounds(to_process,grid_def.gGridDim)){

			uint read_key = SORTING_KEY(to_process,grid_def.gGridDim);

			num[num_count + local_i] = INPUT_COUNT_AT(INPUT_COUNT,INPUT_COUNT_VAR,INPUT_COUNT_SIZE,read_key,INPUT_COUNT_NUM_BUFFER,INPUT_COUNT_INDEX_BUFFER);

			num[num_scan + local_i] = INPUT_SCAN_AT(INPUT_SCAN,INPUT_SCAN_VAR,INPUT_SCAN_SIZE,read_key,INPUT_SCAN_NUM_BUFFER,INPUT_SCAN_INDEX_BUFFER);
		}
		else{
			num[num_count + local_i] = 0;
		}
	}

	memoryBarrierShared();
	barrier();
	PREC_VEC_TYPE vi_mi = PREC_VEC_TYPE(0.0);
	for(int particle_count = 0; particle_count < tile_count; particle_count+=MULTIPLE_PARTICLES){
		//LOAD
		for(int local_i = int(gl_LocalInvocationID.x); local_i < HALO_FLAT;local_i += X*Y*Z) {

			uint batch_count = clamp(int(num[num_count+local_i])-particle_count,0,MULTIPLE_PARTICLES);
#if MULTIPLE_PARTICLES==2
			uint globalParticleIndex = num[num_scan + local_i] +particle_count;
			if(0<batch_count){

				PREC_VEC3_TYPE pos = INPUT_AT(INPUT,Particle_pos_vol,INPUT_SIZE,globalParticleIndex,INPUT_NUM_BUFFER,INPUT_INDEX_BUFFER).xyz;

				temp[temp_pos + local_i ] = PREC_VEC_TYPE((pos-grid_def.gGridPos)/grid_def.gridSpacing,0.0);

				temp[temp_vel + local_i ] = INPUT_AT(INPUT,Particle_vel_mass,INPUT_SIZE,globalParticleIndex,INPUT_NUM_BUFFER,INPUT_INDEX_BUFFER);
			}
			if(1<batch_count){

				globalParticleIndex +=1;

				PREC_VEC3_TYPE pos = INPUT_AT(INPUT,Particle_pos_vol,INPUT_SIZE,globalParticleIndex,INPUT_NUM_BUFFER,INPUT_INDEX_BUFFER).xyz;

				temp[temp_pos + local_i +HALO_FLAT] = PREC_VEC_TYPE((pos-grid_def.gGridPos)/grid_def.gridSpacing,0.0);

				temp[temp_vel + local_i +HALO_FLAT] = INPUT_AT(INPUT,Particle_vel_mass,INPUT_SIZE,globalParticleIndex,INPUT_NUM_BUFFER,INPUT_INDEX_BUFFER);
			}
#else
			for(int particle_i = particle_count; particle_i < particle_count + batch_count;particle_i++){
				uint globalParticleIndex = num[num_scan + local_i] + particle_i;


				PREC_VEC3_TYPE pos = INPUT_AT(INPUT,Particle_pos_vol,INPUT_SIZE,globalParticleIndex,INPUT_NUM_BUFFER,INPUT_INDEX_BUFFER).xyz;

				temp[temp_pos + local_i + HALO_FLAT * (particle_i-particle_count)] = PREC_VEC_TYPE((pos-grid_def.gGridPos)/grid_def.gridSpacing,0.0);

				temp[temp_vel + local_i + HALO_FLAT * (particle_i-particle_count)] = INPUT_AT(INPUT,Particle_vel_mass,INPUT_SIZE,globalParticleIndex,INPUT_NUM_BUFFER,INPUT_INDEX_BUFFER);
			}
#endif
		}

		memoryBarrierShared();
		barrier();
		// TRANSFER
		for(int x = -LEFT_SUPPORT; x<= RIGHT_SUPPORT ;x++){
			for(int y = -LEFT_SUPPORT; y<= RIGHT_SUPPORT ;y++){
				for(int z = -LEFT_SUPPORT; z <= RIGHT_SUPPORT ;z++){
					ivec3 gridOffset = ivec3(x,y,z);
					uint local_i = get_dim_index(t_ijk + uvec3(gridOffset+LEFT_SUPPORT),uvec3(HALO_X,HALO_Y,HALO_Z));
					uint batch_count = clamp(int(num[num_count+local_i])-particle_count,0,MULTIPLE_PARTICLES);
#if MULTIPLE_PARTICLES == 2
					if(0<batch_count){
						PREC_VEC3_TYPE gridDistanceToParticle = vec3(ijk) - temp[temp_pos + local_i].xyz ;
						PREC_SCAL_TYPE wip = .0f;
						weighting (gridDistanceToParticle,wip);

						PREC_VEC_TYPE vp_mp = temp[temp_vel+local_i];
						vi_mi += PREC_VEC_TYPE(vp_mp.xyz,1.0)*vp_mp.w*wip;
					}
					if(1<batch_count){
						PREC_VEC3_TYPE gridDistanceToParticle = vec3(ijk) - temp[temp_pos + local_i + HALO_FLAT].xyz ;
						PREC_SCAL_TYPE wip = .0f;
						weighting (gridDistanceToParticle,wip);

						PREC_VEC_TYPE vp_mp = temp[temp_vel+local_i + HALO_FLAT];
						vi_mi += PREC_VEC_TYPE(vp_mp.xyz,1.0)*vp_mp.w*wip;
					}
#else
					for(int particle_i = 0; particle_i< batch_count;particle_i++){
						PREC_VEC3_TYPE gridDistanceToParticle = vec3(ijk) - temp[temp_pos + local_i + HALO_FLAT * (particle_i)].xyz ;
						PREC_SCAL_TYPE wip = .0f;
						weighting (gridDistanceToParticle,wip);

						PREC_VEC_TYPE vp_mp = temp[temp_vel+local_i + HALO_FLAT * (particle_i)];
						vi_mi += PREC_VEC_TYPE(vp_mp.xyz,1.0)*vp_mp.w*wip;
					}
#endif
				}
			}
		}

		memoryBarrierShared();
		barrier();
	}


	// WRITE
	uint write_key = SORTING_KEY(ijk,grid_def.gGridDim);
	OUTPUT_AT(OUTPUT,Gridpoint_vel_mass,OUTPUT_SIZE,write_key,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER) =
		vi_mi;
	//temp[temp_pos + get_dim_index(t_ijk+LEFT_SUPPORT,uvec3(HALO_X,HALO_Y,HALO_Z))];

}
