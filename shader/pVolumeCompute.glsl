#version 440
uniform vec3 gGridPos;
uniform ivec3 gGridDim;
uniform float gridSpacing;
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


float gCellVolume = gridSpacing *gridSpacing*gridSpacing;
int n= 0;


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
		// mi0 = sum_p [mp *wip0]
		//gxm[gI].w+=mp * wip;

		// pp0 = sum_i[ mi0 *wip0 / h^(3)]
		// atomicAdd(gxm[gI].w, mp * wip);
		//pv[pIndex].w += (mi * wip / gCellVolume)  ;
		float mi = float(gv[gI].w)*1e-8f;
		atomicAdd(pv[pIndex].w, int(
					mi * wip / gCellVolume*1e6f)
			 );

	}

}
