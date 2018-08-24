#version 440
#include "shader/compute/atomic/nvidia.include.glsl"
uniform vec3 gGridPos;
uniform ivec3 gGridDim;
uniform float gridSpacing;
uniform float young;
uniform float poisson;
uniform float hardening;
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
layout(std140, binding = 4) buffer pForceElastic {
	mat4 pFE[ ];
};
layout(std140, binding = 5) buffer pForcePlastic {
	mat4 pFP[ ];
};
layout(std140, binding = 16) buffer gForce {
	vec4 gf[ ];
};
#include "shader/compute/svd/SVD.include.glsl"
#include "shader/compute/interpolation/cubic.include.glsl"
//requires gGridDim uniform
#include "shader/compute/indexing/gridIndex.include.glsl"
#include "shader/compute/indexing/neighborIndex.include.glsl"
#include "shader/compute/lame/snow.include.glsl"


int n= 0;
void main(void){

	uint pIndex= gl_GlobalInvocationID.x;
	uint globalInvocY = gl_GlobalInvocationID.y;

	if (pIndex >= indexSize)
		return;


	//pxm[pIndex].x +=0.00005;

	vec4 particle = pxm[pIndex];
	vec4 particleVelocity = pv[pIndex];
	mat4 FEp4 = mat4(pFE[pIndex]);
	mat3 FEp =mat3(FEp4);
	mat4 FPp4 = mat4(pFP[pIndex]);
	mat3 FPp = mat3(FPp4);

	vec3 xp= particle.xyz; //particle position
	float mp = particle.w; // particle mass
	vec3 vp = particleVelocity.xyz; //particle velocity
	float pp0 = particleVelocity.w; //particle density

	int gridOffsetOfParticle = int(globalInvocY); //  21
	ivec3 gridOffset;

	getIJK(gridOffsetOfParticle,gridOffset ); // temp = 21%16 = 5, ijk=(5%4, 5/4, 21/16) = (1,1,1)+(-2,-2,-2) = (-1,-1,-1)

	// the particle will be scaled to gridSpace
	vec3 ParticleInGrid= (xp- gGridPos)/gridSpacing;

	/* the ijk coordinates in gridSpace are computed by the position of the particle and its offset dependent
	 * on which of the 64 grid nodes is choosen by the invocation
	 */
	ivec3 gridIndex = ivec3(ParticleInGrid) + gridOffset;

	/*
	 * If the referrig gridPoint is out of range of the dimensions of the grid, it will be ignored.
	 */

	if(gridIndex.x>= n && gridIndex.y>=n && gridIndex.z>=n && gridIndex.x< gGridDim[0].x && gridIndex.y <gGridDim[1].x &&gridIndex.z< gGridDim[2].x ){

		vec3 gridDistanceToParticle = vec3(gridIndex)- ParticleInGrid;
		float wip = .0f;
		weighting (gridDistanceToParticle,wip);

		int gI;
		getIndex(gridIndex,gI);

		//min = sum_p [ mp *wipn]
		//gxm[gI].w+= mp * wip;
		atomicAdd(gv[gI].w, mp * wip);

		//vin = sum_p [ vpn * mp *wipn / min]
		//gv[gI].xyz+= vp * mp * wip; // calculate added gridVelocity

		vec3 velocity = (vp * mp * wip);
		atomicAdd(gv[gI].x,velocity.x);
		atomicAdd(gv[gI].y,velocity.y);
		atomicAdd(gv[gI].z,velocity.z);


		mat3 REp, SEp;
		computePD(FEp,REp,SEp);

		for(int i=0; i<3; i++){
			for(int j=0;j<3;j++){
				REp[i][j] =round(1e5f*REp[i][j])/1e5f ;
			}
		}

		//REp =mat3(1.0f);
		float JPp = determinant(FPp);
		float JEp = determinant(FEp);
		vec3 wipg;
		weightingGradient(-gridDistanceToParticle,wipg);
		// fi(^x) = - sum_p [ Vpn * sigmaP * d_wipn]
		//        = - sum_p [ Vp0 * (Jpn * 2 * mu(FPp)/Jpn * (FEp-REp) * FEp^(T) + Jpn* lamba(FPp)/Jpn* (JEp -1.0f) * JEp * FEp^(-T) * FEp^(T))*d_wipn]
		//        = - sum_p [ Vp0  * (2 * mu(FPp) * (FEp-REp) * FEp^(T) + lamba(FPp)* (JEp -1.0f) * JEp * I )*d_wipn]
		if(pp0>.0f){
			// if(gridOffset ==0 &&gridOffset ==0 &&gridOffset ==0){
			vec3 force =-

				(mp/pp0)*

				((  2.0f* mu(JPp)*
				    (FEp-REp)*transpose(FEp)
				    + lambda(JPp)*(JEp -1.0f)*(JEp)* mat3(1.0f)
				 )

				 *

				 wipg)

				;
			//force = wipg;
			//fi[gI].xyz += force;
			atomicAdd(gf[gI].x,force.x);
			atomicAdd(gf[gI].y,force.y);
			atomicAdd(gf[gI].z,force.z);

		}
		}

	}
