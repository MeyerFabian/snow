#version 440
#extension GL_NV_shader_atomic_float: enable

uniform uint indexSize;

#include "shader/compute/interpolation/cubic.include.glsl"
#include "shader/compute/indexing/neighborIndex.include.glsl"
#include "shader/compute/indexing/gridIndex.include.glsl"
layout(local_size_x =X, local_size_y =Y,local_size_z =Z)in;

void main(void){

	uint i = gl_GlobalInvocationID.x;
	if(i>=indexSize){
		return;
	}

	PREC_VEC3_TYPE pos = INPUT_AT(INPUT,Particle_pos_vol,INPUT_SIZE,i,INPUT_NUM_BUFFER,INPUT_INDEX_BUFFER).xyz;


	PREC_VEC_TYPE vp_mp =
		INPUT_AT(INPUT,Particle_vel_mass,INPUT_SIZE,i,INPUT_NUM_BUFFER,INPUT_INDEX_BUFFER);

	// Bin due to position in grid
	PREC_VEC3_TYPE positionInGrid= (pos-grid_def.gGridPos)/grid_def.gridSpacing;


	for(int x = -LEFT_SUPPORT; x<= RIGHT_SUPPORT ;x++){
		for(int y = -LEFT_SUPPORT; y<= RIGHT_SUPPORT ;y++){
			for(int z = -LEFT_SUPPORT; z <= RIGHT_SUPPORT ;z++){
				ivec3 gridOffset = ivec3(x,y,z);

				//floor
				ivec3 globalGridIndex = ivec3(positionInGrid) + gridOffset;
				if(inBounds(globalGridIndex,grid_def.gGridDim)){

					uint voxelAndTileIndex = get_dim_index(globalGridIndex,grid_def.gGridDim);
					vec3 gridDistanceToParticle = vec3(globalGridIndex)- positionInGrid;
					float wip = .0f;
					weighting (gridDistanceToParticle,wip);

					float mp = vp_mp.w;
					vec3 vp = vp_mp.xyz;

					float mi = mp*wip;
					vec3 vi = vp*mp*wip;
					atomicAdd(OUTPUT_AT(OUTPUT,Gridpoint_vel_mass,OUTPUT_SIZE,voxelAndTileIndex,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER).w,
							mi);
					atomicAdd(OUTPUT_AT(OUTPUT,Gridpoint_vel_mass,OUTPUT_SIZE,voxelAndTileIndex,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER).x,
							vi.x);

					atomicAdd(OUTPUT_AT(OUTPUT,Gridpoint_vel_mass,OUTPUT_SIZE,voxelAndTileIndex,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER).y,
							vi.y);

					atomicAdd(OUTPUT_AT(OUTPUT,Gridpoint_vel_mass,OUTPUT_SIZE,voxelAndTileIndex,OUTPUT_NUM_BUFFER,OUTPUT_INDEX_BUFFER).z,
							vi.z);
				}
			}
		}

	}
}
