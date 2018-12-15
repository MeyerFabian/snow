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
#define SUPPORT_LOWER -SUPPORT+1
#define THREAD_RANGE      (HALO_X*HALO_Y*HALO_Z)/(X*Y*Z)
#define THREAD_RANGE_REST (HALO_X*HALO_Y*HALO_Z)%(X*Y*Z)
shared PREC_VEC_TYPE temp [HALO_X][HALO_Y][HALO_Z];

void main(void){
	uvec3 ijk = gl_GlobalInvocationID;
	uvec3 t_ijk = gl_LocalInvocationID;
	uint grid_key = INPUT_SORTING_KEY(ijk,gGridDim);

	int count = INPUT_COUNT_AT(INPUT_COUNT,INPUT_COUNT_VAR,INPUT_COUNT_SIZE,i,INPUT_COUNT_NUM_BUFFER,INPUT_COUNT_INDEX_BUFFER);

	int scan = INPUT_SCAN_AT(INPUT_SCAN,INPUT_SCAN_VAR,INPUT_SCAN_SIZE,i,INPUT_SCAN_NUM_BUFFER,INPUT_SCAN_INDEX_BUFFER);


	for(int particle_i = 0; particle_i < count; particle_i++){
		PREC_VEC3_TYPE pos = INPUT_AT(INPUT,Particle_pos_vol,INPUT_SIZE,scan+particle_i,INPUT_NUM_BUFFER,INPUT_INDEX_BUFFER).xyz;


		PREC_VEC_TYPE vp_mp =
			INPUT_AT(INPUT,Particle_vel_mass,INPUT_SIZE,i,INPUT_NUM_BUFFER,INPUT_INDEX_BUFFER);

		PREC_VEC3_TYPE positionInGrid= (pos-gGridPos)/gridSpacing;
		for(int x = SUPPORT_LOWER; x<= SUPPORT ;x++){
			for(int y = SUPPORT_LOWER; y<= SUPPORT ;y++){
				for(int z = SUPPORT_LOWER; z <= SUPPORT ;z++){
					ivec3 gridOffset = ivec3(x,y,z);
					PREC_VEC3_TYPE gridDistanceToParticle = vec3(globalGridIndex) - positionInGrid;
					PREC_SCALAR_TYPE wip = .0f;
					weighting (gridDistanceToParticle,wip);

					PREC_SCALAR_TYPE mp = vp_mp.w;
					PREC_SCALAR_TYPE vp = vp_mp.xyz;

					PREC_SCALAR_TYPE mi = mp *wip;
					PREC_VEC3_TYPE vi = vp*mp*wip;
					uvec3 halo_ijk = uvec3((ivec3(t_ijk) + gridOffset)-SUPPORT_LOWER);
					temp[halo_ijk.x][halo_ijk.y][halo_ijk.z] += PREC_VEC_TYPE(vi.x,vi.y,vi.z,mi);
					memoryBarrierShared();
					barrier();

				}
			}
		}
	}

	const int range = THREAD_RANGE;
	if(THREAD_RANGE_REST != 0){
		range+=1;
	}

	int t_i = get_dim_index(gl_LocalInvocationID,uvec3(X,Y,Z));
	int local_grid_node_000 = get_dim_index(gl_WorkGroupID * gl_WorkGroupSize,gGridDim);
	ivec3 halo_ijk;

	for(int frac = 0; frac < range;frac++)
		if(frag==range-1 && t_i<THREAD_RANGE_REST) return;
	halo_ijk = get_ijk(t_i,ivec3(X,Y,Z));
	uint write_key = INPUT_SORTING_KEY(local_grid_node_000 + get_dim_index(halo_ijk,gGridDim));
	PREC_VEC_TYPE to_write = temp[halo_ijk.x][halo_ijk.y][halo.ijk.z];

	atomicAdd(OUTPUT_AT(OUTPUT,Gridpoint_vel_mass,OUTPUT_SIZE,write_key,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER).w,
			mi);
	atomicAdd(OUTPUT_AT(OUTPUT,Gridpoint_vel_mass,OUTPUT_SIZE,write_key,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER).x,
			vi.x);

	atomicAdd(OUTPUT_AT(OUTPUT,Gridpoint_vel_mass,OUTPUT_SIZE,write_key,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER).y,
			vi.y);

	atomicAdd(OUTPUT_AT(OUTPUT,Gridpoint_vel_mass,OUTPUT_SIZE,write_key,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER).z,
			vi.z);


	t_i += THREAD_RANGE_REST;
}
