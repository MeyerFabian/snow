#include "shader/shared_hpp/interpolation_support.hpp"
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
