#version 440
#extension GL_NV_shader_atomic_float: enable

#include "shader/compute/interpolation/cubic.include.glsl"
layout(local_size_x =X, local_size_y =Y,local_size_z =Z)in;

#define HALO_X (VOXEL_DIM_X+LEFT_SUPPORT+RIGHT_SUPPORT)
#define HALO_Y (VOXEL_DIM_Y+LEFT_SUPPORT+RIGHT_SUPPORT)
#define HALO_Z (VOXEL_DIM_Z+LEFT_SUPPORT+RIGHT_SUPPORT)
shared PREC_VEC_TYPE temp [HALO_X*HALO_Y*HALO_Z];

#define blockSize uvec3(VOXEL_DIM_X,VOXEL_DIM_Y,VOXEL_DIM_Z)
void main(void){
#ifdef INPUT_BLOCK_INDEX
	uvec3 blockID = getBlockID(INPUT_BLOCK_INDEX_AT(INPUT_BLOCK_INDEX,INPUT_BLOCK_INDEX_VAR,INPUT_BLOCK_INDEX_SIZE,gl_WorkGroupID.x,INPUT_BLOCK_INDEX_NUM_BUFFER,INPUT_BLOCK_INDEX_INDEX_BUFFER), grid_def.gGridDim);
#else
	uvec3 blockID = getBlockID(gl_WorkGroupID.x,grid_def.gGridDim);
#endif

	uvec3 t_ijk = getIJK(gl_LocalInvocationID.x,blockSize);
	uvec3 ijk = blockID * blockSize + t_ijk;

	uint grid_key = SORTING_KEY(ijk,grid_def.gGridDim);
	uint count = OUTPUT_COUNT_AT(OUTPUT_COUNT,OUTPUT_COUNT_VAR,OUTPUT_COUNT_SIZE,grid_key,OUTPUT_COUNT_NUM_BUFFER,OUTPUT_COUNT_INDEX_BUFFER);

	uint scan = OUTPUT_SCAN_AT(OUTPUT_SCAN,OUTPUT_SCAN_VAR,OUTPUT_SCAN_SIZE,grid_key,OUTPUT_SCAN_NUM_BUFFER,OUTPUT_SCAN_INDEX_BUFFER);

	ivec3 grid_start_node = ivec3(blockID * blockSize) - LEFT_SUPPORT;
	for(int local_i = int(gl_LocalInvocationID.x); local_i < (HALO_X*HALO_Y*HALO_Z);local_i += X*Y*Z) {
		uvec3 halo_ijk = uvec3(getIJK(local_i,ivec3(HALO_X,HALO_Y,HALO_Z)));
		ivec3 to_process = grid_start_node + ivec3(halo_ijk);
		if(inBounds(to_process,grid_def.gGridDim)){

			uint read_key = SORTING_KEY(to_process,grid_def.gGridDim);

			temp[local_i] = INPUT_AT(INPUT,Gridpoint_vel_mass,INPUT_SIZE,read_key,INPUT_NUM_BUFFER,INPUT_INDEX_BUFFER);

		}else{
			temp[local_i] = PREC_VEC_TYPE(0.0);
		}
	}

	memoryBarrierShared();
	barrier();

	PREC_VEC3_TYPE vp_n[MULTIPLE_PARTICLES];
	PREC_VEC3_TYPE positionInGrid[MULTIPLE_PARTICLES];
	for(int particle_count = 0; particle_count < count; particle_count+=MULTIPLE_PARTICLES){
		uint batch_count = clamp(int(count)-particle_count,0,MULTIPLE_PARTICLES);
#if MULTIPLE_PARTICLES==2

		uint globalParticleIndex = scan+particle_count;
		vp_n[0]= PREC_VEC3_TYPE(0.0);
		PREC_VEC3_TYPE pos = OUTPUT_AT(OUTPUT,Particle_pos_vol,OUTPUT_SIZE,globalParticleIndex,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER).xyz;

		positionInGrid[0]= (pos-grid_def.gGridPos)/grid_def.gridSpacing;
		if(1<batch_count){

			globalParticleIndex +=1;
			vp_n[1]= PREC_VEC3_TYPE(0.0);
			PREC_VEC3_TYPE pos = OUTPUT_AT(OUTPUT,Particle_pos_vol,OUTPUT_SIZE,globalParticleIndex,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER).xyz;

			positionInGrid[1]= (pos-grid_def.gGridPos)/grid_def.gridSpacing;
		}
#elif MULTIPLE_PARTICLES==4

		uint globalParticleIndex = scan+particle_count;
		vp_n[0]= PREC_VEC3_TYPE(0.0);
		PREC_VEC3_TYPE pos = OUTPUT_AT(OUTPUT,Particle_pos_vol,OUTPUT_SIZE,globalParticleIndex,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER).xyz;

		positionInGrid[0]= (pos-grid_def.gGridPos)/grid_def.gridSpacing;
		if(1<batch_count){

			globalParticleIndex +=1;
			vp_n[1]= PREC_VEC3_TYPE(0.0);
			PREC_VEC3_TYPE pos = OUTPUT_AT(OUTPUT,Particle_pos_vol,OUTPUT_SIZE,globalParticleIndex,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER).xyz;

			positionInGrid[1]= (pos-grid_def.gGridPos)/grid_def.gridSpacing;
		}
		if(2<batch_count){

			globalParticleIndex +=1;
			vp_n[2]= PREC_VEC3_TYPE(0.0);
			PREC_VEC3_TYPE pos = OUTPUT_AT(OUTPUT,Particle_pos_vol,OUTPUT_SIZE,globalParticleIndex,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER).xyz;

			positionInGrid[2]= (pos-grid_def.gGridPos)/grid_def.gridSpacing;
		}
		if(3<batch_count){

			globalParticleIndex +=1;
			vp_n[3]= PREC_VEC3_TYPE(0.0);
			PREC_VEC3_TYPE pos = OUTPUT_AT(OUTPUT,Particle_pos_vol,OUTPUT_SIZE,globalParticleIndex,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER).xyz;

			positionInGrid[3]= (pos-grid_def.gGridPos)/grid_def.gridSpacing;
		}
#else
		for(int particle_i = 0; particle_i < batch_count;particle_i++){
			uint globalParticleIndex = scan+particle_i+particle_count;
			vp_n[particle_i]= PREC_VEC3_TYPE(0.0);
			PREC_VEC3_TYPE pos = OUTPUT_AT(OUTPUT,Particle_pos_vol,OUTPUT_SIZE,globalParticleIndex,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER).xyz;

			positionInGrid[particle_i]= (pos-grid_def.gGridPos)/grid_def.gridSpacing;
		}
#endif
		for(int z = -LEFT_SUPPORT; z<= RIGHT_SUPPORT ;z++){
			for(int y = -LEFT_SUPPORT; y<= RIGHT_SUPPORT ;y++){
				for(int x = -LEFT_SUPPORT; x <= RIGHT_SUPPORT ;x++){
					ivec3 gridOffset = ivec3(x,y,z);
					uvec3 global_grid_index = uvec3(ivec3(ijk)+gridOffset);
					uint local_i = get_dim_index(t_ijk + uvec3(gridOffset +LEFT_SUPPORT ),uvec3(HALO_X,HALO_Y,HALO_Z));
#if MULTIPLE_PARTICLES==2
					PREC_VEC3_TYPE gridDistanceToParticle =vec3(global_grid_index) -  positionInGrid[0] ;
					PREC_SCAL_TYPE wip = 0.0f;
					weighting (gridDistanceToParticle,wip);

					PREC_VEC_TYPE vi_n =  temp[local_i];

					vp_n[0] += wip * vi_n.xyz;

					if(1 < batch_count){
						PREC_VEC3_TYPE gridDistanceToParticle =vec3(global_grid_index) -  positionInGrid[1] ;
						PREC_SCAL_TYPE wip = 0.0f;
						weighting (gridDistanceToParticle,wip);

						PREC_VEC_TYPE vi_n =  temp[local_i];

						vp_n[1] += wip * vi_n.xyz;
					}
#elif MULTIPLE_PARTICLES ==4
					PREC_VEC3_TYPE gridDistanceToParticle =vec3(global_grid_index) -  positionInGrid[0] ;
					PREC_SCAL_TYPE wip = 0.0f;
					weighting (gridDistanceToParticle,wip);

					PREC_VEC_TYPE vi_n =  temp[local_i];

					vp_n[0] += wip * vi_n.xyz;

					if(1 < batch_count){
						PREC_VEC3_TYPE gridDistanceToParticle =vec3(global_grid_index) -  positionInGrid[1] ;
						PREC_SCAL_TYPE wip = 0.0f;
						weighting (gridDistanceToParticle,wip);

						PREC_VEC_TYPE vi_n =  temp[local_i];

						vp_n[1] += wip * vi_n.xyz;
					}
					if(2 < batch_count){
						PREC_VEC3_TYPE gridDistanceToParticle =vec3(global_grid_index) -  positionInGrid[2] ;
						PREC_SCAL_TYPE wip = 0.0f;
						weighting (gridDistanceToParticle,wip);

						PREC_VEC_TYPE vi_n =  temp[local_i];

						vp_n[2] += wip * vi_n.xyz;
					}
					if(3 < batch_count){
						PREC_VEC3_TYPE gridDistanceToParticle =vec3(global_grid_index) -  positionInGrid[3] ;
						PREC_SCAL_TYPE wip = 0.0f;
						weighting (gridDistanceToParticle,wip);

						PREC_VEC_TYPE vi_n =  temp[local_i];

						vp_n[3] += wip * vi_n.xyz;
					}
#else
					for(int particle_i = 0; particle_i < batch_count;particle_i++){
						PREC_VEC3_TYPE gridDistanceToParticle =vec3(global_grid_index) -  positionInGrid[particle_i] ;
						PREC_SCAL_TYPE wip = 0.0f;
						weighting (gridDistanceToParticle,wip);

						PREC_VEC_TYPE vi_n =  temp[local_i];

						vp_n[particle_i] += wip * vi_n.xyz;
					}
#endif
				}
			}
		}

#if MULTIPLE_PARTICLES==2
		globalParticleIndex = scan+particle_count;
		OUTPUT_AT(OUTPUT,Particle_vel_mass,OUTPUT_SIZE,globalParticleIndex,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER).xyz = vp_n[0];

		if(1 < batch_count){
			globalParticleIndex += 1;
			OUTPUT_AT(OUTPUT,Particle_vel_mass,OUTPUT_SIZE,globalParticleIndex,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER).xyz = vp_n[1];
		}
#elif MULTIPLE_PARTICLES==4
		globalParticleIndex = scan+particle_count;
		OUTPUT_AT(OUTPUT,Particle_vel_mass,OUTPUT_SIZE,globalParticleIndex,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER).xyz = vp_n[0];

		if(1 < batch_count){
			globalParticleIndex += 1;
			OUTPUT_AT(OUTPUT,Particle_vel_mass,OUTPUT_SIZE,globalParticleIndex,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER).xyz = vp_n[1];
		}
		if(2 < batch_count){
			globalParticleIndex += 1;
			OUTPUT_AT(OUTPUT,Particle_vel_mass,OUTPUT_SIZE,globalParticleIndex,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER).xyz = vp_n[2];
		}
		if(3 < batch_count){
			globalParticleIndex += 1;
			OUTPUT_AT(OUTPUT,Particle_vel_mass,OUTPUT_SIZE,globalParticleIndex,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER).xyz = vp_n[3];
		}
#else
		for(int particle_i = 0; particle_i < batch_count;particle_i++){
			uint globalParticleIndex = scan+particle_i+particle_count;
			OUTPUT_AT(OUTPUT,Particle_vel_mass,OUTPUT_SIZE,globalParticleIndex,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER).xyz = vp_n[particle_i];
		}
#endif
	}
}
