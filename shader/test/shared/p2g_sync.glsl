#version 440
#extension GL_NV_shader_atomic_float: enable

uniform vec3 gGridPos;
uniform uvec3 gGridDim;
uniform float gridSpacing;

uniform uint indexSize;

#include "shader/compute/interpolation/cubic.include.glsl"
#include "shader/compute/indexing/neighborIndex.include.glsl"
#include "shader/compute/indexing/gridIndex.include.glsl"
layout(local_size_x =X, local_size_y =Y,local_size_z =Z)in;

#define HALO_X (X+2*SUPPORT)
#define HALO_Y (Y+2*SUPPORT)
#define HALO_Z (Z+2*SUPPORT)
#define THREAD_RANGE      (HALO_X*HALO_Y*HALO_Z)/(X*Y*Z)
#define THREAD_RANGE_REST (HALO_X*HALO_Y*HALO_Z)%(X*Y*Z)
shared PREC_VEC_TYPE temp [HALO_X][HALO_Y][HALO_Z];

void main(void){
	uvec3 ijk = gl_GlobalInvocationID;
	uvec3 t_ijk = gl_LocalInvocationID;
	uint grid_key = INPUT_SORTING_KEY(ijk,gGridDim);

	uint count = INPUT_COUNT_AT(INPUT_COUNT,INPUT_COUNT_VAR,INPUT_COUNT_SIZE,grid_key,INPUT_COUNT_NUM_BUFFER,INPUT_COUNT_INDEX_BUFFER);

	uint scan = INPUT_SCAN_AT(INPUT_SCAN,INPUT_SCAN_VAR,INPUT_SCAN_SIZE,grid_key,INPUT_SCAN_NUM_BUFFER,INPUT_SCAN_INDEX_BUFFER);

	for(int particle_i = 0; particle_i < count; particle_i++){
		uint globalParticleIndex = scan+particle_i;
		PREC_VEC3_TYPE pos = INPUT_AT(INPUT,Particle_pos_vol,INPUT_SIZE,globalParticleIndex,INPUT_NUM_BUFFER,INPUT_INDEX_BUFFER).xyz;


		PREC_VEC_TYPE vp_mp =
			INPUT_AT(INPUT,Particle_vel_mass,INPUT_SIZE,globalParticleIndex,INPUT_NUM_BUFFER,INPUT_INDEX_BUFFER);

		PREC_VEC3_TYPE positionInGrid= (pos-gGridPos)/gridSpacing;
		for(int x = -SUPPORT; x<= SUPPORT ;x++){
			for(int y = -SUPPORT; y<= SUPPORT ;y++){
				for(int z = -SUPPORT; z <= SUPPORT ;z++){
					ivec3 gridOffset = ivec3(x,y,z);
					PREC_VEC3_TYPE gridDistanceToParticle = positionInGrid - vec3(ijk+gridOffset);
					PREC_SCAL_TYPE wip = .0f;
					weighting (gridDistanceToParticle,wip);

					PREC_SCAL_TYPE mp = vp_mp.w;
					PREC_VEC3_TYPE vp = vp_mp.xyz;

					PREC_SCAL_TYPE mi = mp *wip;
					PREC_VEC3_TYPE vi = vp*mp*wip;
					uvec3 halo_ijk = uvec3((ivec3(t_ijk) + gridOffset)+SUPPORT);
					temp[halo_ijk.x][halo_ijk.y][halo_ijk.z]+=
						PREC_VEC_TYPE(1.0/64.0,1.0/64.0,1.0/64.0,wip);
					//temp[halo_ijk.x][halo_ijk.y][halo_ijk.z] +=
					// PREC_VEC_TYPE(vi.x,vi.y,vi.z,mi);
					memoryBarrierShared();
					barrier();

				}
			}
		}
	}

	memoryBarrierShared();
	barrier();
	int range = THREAD_RANGE;

	int local_i = int(get_dim_index(gl_LocalInvocationID,uvec3(X,Y,Z)));

	ivec3 grid_start_node = ivec3(gl_WorkGroupID * gl_WorkGroupSize) - SUPPORT;

	for(int frac = 0; frac < range;frac++){
		ivec3 halo_ijk = getIJK(local_i,ivec3(HALO_X,HALO_Y,HALO_Z));
		ivec3 to_process = grid_start_node + halo_ijk;
		to_process = clamp(to_process, ivec3(0,0,0), ivec3(gGridDim-1));
		if(inBounds(to_process,gGridDim)){
			uint write_key = INPUT_SORTING_KEY(to_process,gGridDim);
			PREC_VEC_TYPE to_write = vec4(1.0,1.0,1.0,1.0);// temp[halo_ijk.x][halo_ijk.y][halo_ijk.z];

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

	if(local_i < (range)*(X*Y*Z) + THREAD_RANGE_REST) {
		ivec3 halo_ijk = getIJK(local_i,ivec3(HALO_X,HALO_Y,HALO_Z));
		ivec3 to_process = grid_start_node + halo_ijk;
		to_process = clamp(to_process, ivec3(0,0,0), ivec3(gGridDim-1));
		if(inBounds(to_process,gGridDim)){
			uint write_key = INPUT_SORTING_KEY(to_process,gGridDim);
			PREC_VEC_TYPE to_write = vec4(1.0,1.0,1.0,1.0);// temp[halo_ijk.x][halo_ijk.y][halo_ijk.z];

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
