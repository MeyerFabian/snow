#version 440
#extension GL_ARB_compute_variable_group_size :require

uniform vec3 gGridPos;
uniform ivec3 gGridDim;
uniform float gridSpacing;


layout(local_size_variable)in;
//try using y
//layout(local_size_x= 1024, local_size_y = 1, local_size_z = 1)in;

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
    mat3 pFE[ ];
};
layout(std140, binding = 5) buffer pForcePlastic {
    mat3 pFP[ ];
};
layout(std140, binding = 6) buffer gForce{
    vec4 fi[] ;
};

int n= 0;

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
    if(absX < 1){
        return 0.5f *absX*absX*absX -x*x +2.0f/3.0f;
    }
    else if (absX <= 2){
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


void main(void){

    uint pIndex= gl_GlobalInvocationID.x;
    uint globalInvocY = gl_GlobalInvocationID.y;

    pxm[pIndex].x +=0.00005;

    vec4 particle = pxm[pIndex];
    vec4 particleVelocity = pv[pIndex];
    mat3 FEp = pFE[pIndex];

    vec3 xp= particle.xyz; //particle position
    float mp = particle.w; // particle mass
    vec3 vp = particleVelocity.xyz; //particle velocity
    //float Vp = particleVelocity.w; //particle Volume

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

        vec3 gridDistanceToParticle = ParticleInGrid- vec3(gridIndex);
        float wip = .0f;
        weighting (gridDistanceToParticle,wip);

        int gI;
        getIndex(gridIndex,gI);

        gxm[gI].w+= mp * wip; // add ParticleMass to gridPointMass

        barrier();
        float mi= gxm[gI].w;

        gv[gI].xyz+= vp * mp * wip / mi; // calculate added gridVelocity


   }

}
