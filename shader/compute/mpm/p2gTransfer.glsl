#version 440
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
	ivec4 pv[ ];
};

layout(std140, binding = 2) buffer gPosMass {
	vec4 gxm[ ];
};
layout(std140, binding = 3) buffer gVel {
	ivec4 gv[ ];
};
layout(std140, binding = 4) buffer pForceElastic {
	mat4 pFE[ ];
};
layout(std140, binding = 5) buffer pForcePlastic {
	mat4 pFP[ ];
};
layout(std140, binding = 16) buffer gForce {
	ivec4 gf[ ];
};

#include "shader/compute/svd/SVD.include.glsl"

/*
 * E0 Young Modulus
 * r Poissons ratio
 * Es hardening coefficient
 * Jp determinant of FPp
 * mu0 = E0/(2*(1+r))
 * mu = mu0 * e(Es(1-Jp))
 * JP=1.1, 1. Initial Parameters: 140000 /(2*(1+0.2))*e(10*(1.0-1.1) = 21459.63
 * JP=0.9, 2. Initial Parameters: 140000 /(2*(1+0.2))*e(10*(1.0-0.9) = 158566.43
 */
float mu(const float JP){
	return young /(2.0f*(1.0f+poisson))* exp(hardening*(1.0f-JP));
}

/*
 * E0 Young Modulus
 * r Poissons ratio
 * Es hardening coefficient
 * Jp determinant of FPp
 * lambda0 = E0*r/((1+r)(1-2r))
 * lambda = lambda0 * e(Es(1-Jp))
 * JP = 1.1,Initial Parameters: 140000*0.2/((1.0+0.2)*(1.9-2.0*0.2))*e^(10*(1-1.1)) = 5722.56
 * JP = 0.9, Initial Parameters: 140000*0.2/((1.0+0.2)*(1.9-2.0*0.2))*e^(10*(1-0.9)) = 42284.38
 */
float lambda(const float JP){
	return young*poisson /((1.0f+poisson)*(1.0f-2.0f*poisson))* exp(hardening*(1.0f-JP));
}


/**
 * Takes an integer vector ijk and returns the respective buffer index.
 * i of [0,x-GridDimension]
 * j of[0,y-GridDimension]
 * k of [0, z-GridDimension]
 */
void getIndex(const ivec3 ijk,inout int index){
	index = ijk.x + (ijk.y * int(gGridDim[0].x)) + (ijk.z *int(gGridDim[1].x) * int(gGridDim[0].x));
}

/**
 * Takes an index with range [0,63] referring to one of the 64 neighbors
 * (4x*4y*4z = 64 Neighbors) and returns its relative signed and rounded position to
 * that grid node.
 */
int width = 4;
ivec3 windowOffset=ivec3(-1,-1,-1);

void getIJK(const  int index,inout ivec3 ijk){
	int temp = index%(width*width);
	ijk= ivec3(temp%width,temp/width,index/(width*width))+windowOffset;
}


/*
 * Returns weight distribution by grid basis function (dyadic products of one-dimensional
 * cubic B-splines) from particle to actual grid neighbors dependant on their distance to the particle.
 */

float weighting(const float x){
	const float absX = abs(x);
	if(absX < 1.0f){
		return 0.5f *absX*absX*absX -x*x +2.0f/3.0f;
	}
	else if (absX < 2.0f){
		return -1.0f/6.0f *absX*absX*absX +x*x - 2.0f *absX + 4.0f/3.0f;
	}
	return 0.0f;
}

/**
 * Weighting is split into x,y and z direction.
 */
void weighting(const vec3 distanceVector, inout float w){
	w = weighting(distanceVector.x)*  weighting(distanceVector.y) * weighting(distanceVector.z);
}

float weightingGradient(const float x){
	const float absX = abs(x);
	if(absX < 1.0f){
		return 1.5f *x*absX-2.0f*x;
	}
	else if (absX < 2.0f){
		return -1.0f/2.0f *absX*x + 2.0f*x - 2.0f*x/absX;
	}
	return 0.0f;
}

void weightingGradient(const vec3 distanceVector, inout vec3 wg){
	wg.x = weightingGradient(distanceVector.x)*  weighting(distanceVector.y) * weighting(distanceVector.z);
	wg.y = weighting(distanceVector.x)*  weightingGradient(distanceVector.y) * weighting(distanceVector.z);
	wg.z = weighting(distanceVector.x)*  weighting(distanceVector.y) * weightingGradient(distanceVector.z);
}

int n= 0;
void main(void){

	uint pIndex= gl_GlobalInvocationID.x;
	uint globalInvocY = gl_GlobalInvocationID.y;

	if (pIndex >= indexSize)
		return;


	//pxm[pIndex].x +=0.00005;

	vec4 particle = pxm[pIndex];
	vec4 particleVelocity = vec4(pv[pIndex]);
	mat4 FEp4 = mat4(pFE[pIndex]);
	mat3 FEp =mat3(FEp4);
	mat4 FPp4 = mat4(pFP[pIndex]);
	mat3 FPp = mat3(FPp4);

	vec3 xp= particle.xyz; //particle position
	float mp = particle.w; // particle mass
	if (abs(mp) > 100.0) {
		return;
	}
	vec3 vp = vec3(particleVelocity.xyz)*1e-8f; //particle velocity
	float pp0 = float(particleVelocity.w)*1e-6f; //particle density

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
		atomicAdd(gv[gI].w,  int(mp * wip* 1e8f));

		//vin = sum_p [ vpn * mp *wipn / min]
		//gv[gI].xyz+= vp * mp * wip; // calculate added gridVelocity

		vec3 velocity = (vp * mp * wip);
		atomicAdd(gv[gI].x,int(velocity.x*1e8f));
		atomicAdd(gv[gI].y,int(velocity.y*1e8f));
		atomicAdd(gv[gI].z,int(velocity.z*1e8f));


		mat3 REp, SEp;
		computePD(FEp,REp,SEp);

		for(int i=0; i<3; i++){
			for(int j=0;j<3;j++){
				REp[i][j] =round(1e9f*REp[i][j])/1e9f ;
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
		if(pp0>0.0f){
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
			atomicAdd(gf[gI].x,int(force.x*1e6f));
			atomicAdd(gf[gI].y,int(force.y*1e6f));
			atomicAdd(gf[gI].z,int(force.z*1e6f));

		}
		}

	}
