#version 440
#extension GL_NV_shader_atomic_float: enable
uniform vec3 gGridPos;
uniform ivec3 gGridDim;
uniform float gridSpacing;
uniform uint indexSize;

layout(local_size_x =1024, local_size_y =1,local_size_z =1)in;

layout(std140, binding = 0) buffer pPosMass {
	vec4 pxm[ ];
};

layout(std140, binding = 1) buffer pVelVolume {
	vec4 pv[ ];
};

layout(std140, binding = 2) buffer gPosMass {
	vec4 gxm[ ];
};
layout(std140, binding = 3) buffer gVel {
	vec4 gv[ ];
};

#include "shader/compute/interpolation/cubic.include.glsl"
//requires gGridDim uniform
#include "shader/compute/indexing/gridIndex.include.glsl"
#include "shader/compute/indexing/neighborIndex.include.glsl"

float gCellVolume = gridSpacing *gridSpacing*gridSpacing;
int n= 0;

void main(void){

	uint pIndex= gl_GlobalInvocationID.x;
	uint globalInvocY = gl_GlobalInvocationID.y;

	if (pIndex >= indexSize)
		return;

	vec4 particle = pxm[pIndex];
	vec4 particleVelocity = pv[pIndex];

	vec3 xp= particle.xyz; //particle position
	float mp = particle.w; // particle mass

	int gI = 0;

	int gridOffsetOfParticle = int(globalInvocY); //  597%64=21
	ivec3 gridOffset;

	getIJK(gridOffsetOfParticle,gridOffset ); // temp = 21%16 = 5, ijk=(5%4, 5/4, 21/16) = (1,1,1)+(-2,-2,-2) = (-1,-1,-1)

	vec3 ParticleInGrid= (xp- gGridPos)/gridSpacing;

	ivec3 gridIndex = ivec3(ParticleInGrid) + gridOffset;


	// gridIndex= ivec3(((pxm[9].xyz - (-1.05,0.05,-1.05))/0.1) + (-1,-1,-1)
	//          = ivec3(((0.4,0.5,-0.5)-(-1.05,0.05,-1.05))/0.1))+(-1,-1,-1)
	//          = (1.45,0.55,0.55)/0.1 +(-1,-1,-1)
	//          = (14.5,5.5,5.5) +(-1,-1,-1)
	//          = (13.5,4.5,4.5)
	if(gridIndex.x>= n && gridIndex.y>=n && gridIndex.z>=n && gridIndex.x< gGridDim[0].x && gridIndex.y <gGridDim[1].x &&gridIndex.z< gGridDim[2].x ){

		vec3 gridDistanceToParticle = vec3(gridIndex)- ParticleInGrid;
		float wip = .0f;
		weighting (gridDistanceToParticle,wip);

		getIndex(gridIndex,gI);
		atomicAdd(gv[gI].w, mp * wip);


	}

}
