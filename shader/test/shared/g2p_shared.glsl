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

	for(int particle_i = 0; particle_i < count; particle_i++){
		PREC_VEC3_TYPE vp_n= PREC_VEC3_TYPE(0.0);
#ifdef APIC
		PREC_MAT3_TYPE Bp_n = PREC_MAT3_TYPE(0.0);
#endif
		uint globalParticleIndex = scan+particle_i;
		PREC_VEC3_TYPE pos = OUTPUT_AT(OUTPUT,Particle_pos_vol,OUTPUT_SIZE,globalParticleIndex,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER).xyz;

		PREC_VEC3_TYPE positionInGrid= (pos-grid_def.gGridPos)/grid_def.gridSpacing;
		for(int z = -LEFT_SUPPORT; z<= RIGHT_SUPPORT ;z++){
			for(int y = -LEFT_SUPPORT; y<= RIGHT_SUPPORT ;y++){
				for(int x = -LEFT_SUPPORT; x <= RIGHT_SUPPORT ;x++){
					ivec3 gridOffset = ivec3(x,y,z);
					uvec3 global_grid_index = uvec3(ivec3(ijk)+gridOffset);
					uint local_i = get_dim_index(t_ijk + uvec3(gridOffset +LEFT_SUPPORT ),uvec3(HALO_X,HALO_Y,HALO_Z));
					PREC_VEC3_TYPE gridDistanceToParticle =vec3(global_grid_index) -  positionInGrid ;
					PREC_SCAL_TYPE wip = 0.0f;
					weighting (gridDistanceToParticle,wip);

					PREC_VEC_TYPE vi_n =  temp[local_i];

					vp_n += wip * vi_n.xyz;
#ifdef APIC
					//need to be reworked
					PREC_VEC3_TYPE delta_x_n = (gridDistanceToParticle+gridDistanceToParticle)*grid_def.gridSpacing;
					PREC_VEC3_TYPE delta_x_p = (gridDistanceToParticle-gridDistanceToParticle)*grid_def.gridSpacing;

					Bp_n += 0.5 * wip * (outerProduct(vi_n.xyz,delta_x_n) + outerProduct(delta_x_p,vi_n.xyz));
#endif
				}
			}
		}
		OUTPUT_AT(OUTPUT,Particle_vel_mass,OUTPUT_SIZE,globalParticleIndex,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER).xyz = vp_n;
#ifdef APIC
		OUTPUT_AT(OUTPUT,Particle_Bp_1,OUTPUT_SIZE,globalParticleIndex,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER).xyz = Bp_n[0];
		OUTPUT_AT(OUTPUT,Particle_Bp_2,OUTPUT_SIZE,globalParticleIndex,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER).xyz = Bp_n[1];
		OUTPUT_AT(OUTPUT,Particle_Bp_3,OUTPUT_SIZE,globalParticleIndex,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER).xyz = Bp_n[2];
#endif
	}
}
