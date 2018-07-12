#version 440
#define alpha 0.95
uniform vec3 gGridPos;
uniform ivec3 gGridDim;
uniform float gridSpacing;
uniform float young;
uniform float poisson;
uniform float hardening;
uniform float critComp;
uniform float critStretch;
uniform int indexSize;


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

layout(std140, binding = 7) buffer gVeln {
    vec4 gvn[ ];
};
layout(std140, binding = 6) buffer pVeln {
    ivec4 pvn[ ];
};
layout(std140, binding = 13) buffer pDeltaVel0{
    ivec4 pdvp0[];
};
layout(std140, binding = 14) buffer pDeltaVel1{
    ivec4 pdvp1[];
};
layout(std140, binding = 15) buffer pDeltaVel2{
    ivec4 pdvp2[];
};
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


    vec4 particle = pxm[pIndex];
    vec4 particleVelocity = vec4(pv[pIndex]);

    vec3 xp= particle.xyz; //particle position

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

        vec3 gwip =vec3(.0f);
        weightingGradient(-gridDistanceToParticle,gwip);
        int gI;
        getIndex(gridIndex,gI);

        vec3 vin =gvn[gI].xyz;
        vec3 vi =vec3(gv[gI].xyz)*1e-8f; //needs to be normalized with mi
        float mi = float(gv[gI].w)*1e-8f;
        //vpn+1 = a * vpn + temp_vpn+1
        //temp_vpn+1 = sum_i [(1-a) * vin+1 * wipn + (a) * (vin+1 - vin)* wipn]
        if(mi>0.0f){
        vec3 vpn = ((1.0f-alpha) *vin * wip)+(alpha*(vin-vi/mi)*wip); // add ParticleMass to gridPointMass
        //fi[gI].xyz += force;
        atomicAdd(pvn[gl_GlobalInvocationID.x].x,int(vpn.x*1e8f));
        atomicAdd(pvn[gl_GlobalInvocationID.x].y,int(vpn.y*1e8f));
        atomicAdd(pvn[gl_GlobalInvocationID.x].z,int(vpn.z*1e8f));

        //pvn[gl_GlobalInvocationID.x].xyz +=vin*wip;
        //d_vpn+1 = sum_i [vin+1 * d_wipn^(T)]
        /*
        atomicAdd(deltapvn0[gl_GlobalInvocationID.x].x,vin.x * gwip.x);
        atomicAdd(deltapvn0[gl_GlobalInvocationID.x].y,vin.y * gwip.x);
        atomicAdd(deltapvn0[gl_GlobalInvocationID.x].z,vin.z * gwip.x);
        atomicAdd(deltapvn1[gl_GlobalInvocationID.x].x,vin.x * gwip.y);
        atomicAdd(deltapvn1[gl_GlobalInvocationID.x].y,vin.y * gwip.y);
        atomicAdd(deltapvn1[gl_GlobalInvocationID.x].z,vin.z * gwip.y);
        atomicAdd(deltapvn2[gl_GlobalInvocationID.x].x,vin.x * gwip.z);
        atomicAdd(deltapvn2[gl_GlobalInvocationID.x].y,vin.y * gwip.z);
        atomicAdd(deltapvn2[gl_GlobalInvocationID.x].z,vin.z * gwip.z);

*/

        atomicAdd(pdvp0[gl_GlobalInvocationID.x].x,int(vin.x * gwip.x*1e8f));
        atomicAdd(pdvp0[gl_GlobalInvocationID.x].y,int(vin.y * gwip.x*1e8f));
        atomicAdd(pdvp0[gl_GlobalInvocationID.x].z,int(vin.z * gwip.x*1e8f));
        atomicAdd(pdvp1[gl_GlobalInvocationID.x].x,int(vin.x * gwip.y*1e8f));
        atomicAdd(pdvp1[gl_GlobalInvocationID.x].y,int(vin.y * gwip.y*1e8f));
        atomicAdd(pdvp1[gl_GlobalInvocationID.x].z,int(vin.z * gwip.y*1e8f));
        atomicAdd(pdvp2[gl_GlobalInvocationID.x].x,int(vin.x * gwip.z*1e8f));
        atomicAdd(pdvp2[gl_GlobalInvocationID.x].y,int(vin.y * gwip.z*1e8f));
        atomicAdd(pdvp2[gl_GlobalInvocationID.x].z,int(vin.z * gwip.z*1e8f));
}
        /*
        deltapvn[gl_GlobalInvocationID.x][0][0] += mat4( vin.x * gwip.x,vin.x * gwip.y, vin.x *gwip.z,0.0f,
                                                   vin.y * gwip.x,vin.y * gwip.y, vin.y *gwip.z,0.0f,
                                                   vin.z * gwip.x,vin.z * gwip.y, vin.z *gwip.z,0.0f,
                                                   0.0f,0.0f,0.0f,0.0f);
/*
        deltapvn[gl_GlobalInvocationID.x] += mat4( 0.0f,0.0f, 0.0f,0.0f,
                                                   0.0f,0.0f, 0.0f,0.0f,
                                                   0.0f,0.0f, 0.0f,0.0f,
                                                   0.0f,0.0f,0.0f,0.0f);*/
    }

}
