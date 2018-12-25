#version 440
#extension GL_NV_shader_atomic_float: enable


uniform uint indexSize;
uniform uint ParticleMaxCount;

#include "shader/compute/interpolation/cubic.include.glsl"
#include "shader/compute/indexing/neighborIndex.include.glsl"
#include "shader/compute/indexing/gridIndex.include.glsl"
layout(local_size_x =X, local_size_y =Y,local_size_z =Z)in;

#define HALO_X (X+LEFT_SUPPORT+RIGHT_SUPPORT)
#define HALO_Y (Y+LEFT_SUPPORT+RIGHT_SUPPORT)
#define HALO_Z (Z+LEFT_SUPPORT+RIGHT_SUPPORT)
#define THREAD_RANGE      (HALO_X*HALO_Y*HALO_Z)/(X*Y*Z)
#define THREAD_RANGE_REST (HALO_X*HALO_Y*HALO_Z)%(X*Y*Z)
shared PREC_VEC_TYPE temp [HALO_X][HALO_Y][HALO_Z];

void main(void){
	uvec3 ijk = gl_GlobalInvocationID;
	uvec3 t_ijk = gl_LocalInvocationID;
	uint grid_key = INPUT_SORTING_KEY(ijk,grid_def.gGridDim);
	uint count = INPUT_COUNT_AT(INPUT_COUNT,INPUT_COUNT_VAR,INPUT_COUNT_SIZE,grid_key,INPUT_COUNT_NUM_BUFFER,INPUT_COUNT_INDEX_BUFFER);

	uint scan = INPUT_SCAN_AT(INPUT_SCAN,INPUT_SCAN_VAR,INPUT_SCAN_SIZE,grid_key,INPUT_SCAN_NUM_BUFFER,INPUT_SCAN_INDEX_BUFFER);

	int local_i = int(get_dim_index(gl_LocalInvocationID,uvec3(X,Y,Z)));
	for(int frac = 0; frac < THREAD_RANGE;frac++){
		ivec3 halo_ijk = getIJK(local_i,ivec3(HALO_X,HALO_Y,HALO_Z));
		temp[halo_ijk.x][halo_ijk.y][halo_ijk.z] = PREC_VEC_TYPE(0.0);
		local_i += X*Y*Z;
	}

	if(local_i < (THREAD_RANGE)*(X*Y*Z) + THREAD_RANGE_REST) {
		ivec3 halo_ijk = getIJK(local_i,ivec3(HALO_X,HALO_Y,HALO_Z));
		temp[halo_ijk.x][halo_ijk.y][halo_ijk.z] = PREC_VEC_TYPE(0.0);
	}

	memoryBarrierShared();
	barrier();
	for(int particle_i = 0; particle_i < ParticleMaxCount; particle_i++){

		PREC_VEC3_TYPE pos;
		PREC_VEC_TYPE vp_mp;
		uint globalParticleIndex;
		PREC_VEC3_TYPE positionInGrid;
		if(particle_i < count){
			globalParticleIndex= scan+particle_i;

			pos= INPUT_AT(INPUT,Particle_pos_vol,INPUT_SIZE,globalParticleIndex,INPUT_NUM_BUFFER,INPUT_INDEX_BUFFER).xyz;



			vp_mp=	INPUT_AT(INPUT,Particle_vel_mass,INPUT_SIZE,globalParticleIndex,INPUT_NUM_BUFFER,INPUT_INDEX_BUFFER);

			positionInGrid= (pos-grid_def.gGridPos)/grid_def.gridSpacing;
		}
		for(int i=0; i<64;i++){
			if(particle_i < count){
				uvec3 halo_ijk = getIJK(i,ivec3(4,4,4));
				ivec3 gridOffset = ivec3(halo_ijk)-LEFT_SUPPORT;
				halo_ijk += t_ijk;
				PREC_VEC3_TYPE gridDistanceToParticle =vec3(ijk+gridOffset) -  positionInGrid ;
				PREC_SCAL_TYPE wip = .0f;
				weighting (gridDistanceToParticle,wip);

				PREC_SCAL_TYPE mp = vp_mp.w;
				PREC_VEC3_TYPE vp = vp_mp.xyz;

				PREC_SCAL_TYPE mi = mp ;//*wip;
				PREC_VEC3_TYPE vi = vp*mp*wip;
				temp[halo_ijk.x][halo_ijk.y][halo_ijk.z] += vec4(vi,mi);
			}
			memoryBarrierShared();
			barrier();
		}
	}
	memoryBarrierShared();
	barrier();

	local_i = int(get_dim_index(gl_LocalInvocationID,uvec3(X,Y,Z)));
	ivec3 grid_start_node = ivec3(gl_WorkGroupID * gl_WorkGroupSize) - LEFT_SUPPORT;

	for(int frac = 0; frac < THREAD_RANGE;frac++){
		ivec3 halo_ijk = getIJK(local_i,ivec3(HALO_X,HALO_Y,HALO_Z));
		ivec3 to_process = grid_start_node + halo_ijk;
		if(inBounds(to_process,grid_def.gGridDim)){
			uint write_key = INPUT_SORTING_KEY(to_process,grid_def.gGridDim);
			PREC_VEC_TYPE to_write = temp[halo_ijk.x][halo_ijk.y][halo_ijk.z];

			atomicAdd(OUTPUT_AT(OUTPUT,Gridpoint_vel_mass,OUTPUT_SIZE,write_key,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER).x,
					to_write.x);

			atomicAdd(OUTPUT_AT(OUTPUT,Gridpoint_vel_mass,OUTPUT_SIZE,write_key,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER).y,
					to_write.y);

			atomicAdd(OUTPUT_AT(OUTPUT,Gridpoint_vel_mass,OUTPUT_SIZE,write_key,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER).z,
					to_write.z);

			atomicAdd(OUTPUT_AT(OUTPUT,Gridpoint_vel_mass,OUTPUT_SIZE,write_key,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER).w,
					to_write.w);
		}
		local_i += X*Y*Z;
	}

	if(local_i < (THREAD_RANGE)*(X*Y*Z) + THREAD_RANGE_REST) {
		ivec3 halo_ijk = getIJK(local_i,ivec3(HALO_X,HALO_Y,HALO_Z));
		ivec3 to_process = grid_start_node + halo_ijk;
		if(inBounds(to_process,grid_def.gGridDim)){
			uint write_key = INPUT_SORTING_KEY(to_process,grid_def.gGridDim);
			PREC_VEC_TYPE to_write = temp[halo_ijk.x][halo_ijk.y][halo_ijk.z];

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
