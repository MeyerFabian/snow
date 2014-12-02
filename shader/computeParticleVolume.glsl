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

int width = 4;
ivec3 windowOffset=ivec3(-1,-1,-1);
float gCellVolume = gridSpacing *gridSpacing*gridSpacing;
int n= 0;

void getIndex(const ivec3 ijk,inout int index){
    index = ijk.x + (ijk.y * int(gGridDim[0].x)) + (ijk.z *int(gGridDim[1].x) * int(gGridDim[0].x));
}
void getIJK(const  int index,inout ivec3 ijk){
    int temp = index%(width*width);
    ijk= ivec3(temp%width,temp/width,index/(width*width))+windowOffset;
}

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


void weighting(const vec3 distanceVector, inout float w){
    w = weighting(distanceVector.x)*  weighting(distanceVector.y) * weighting(distanceVector.z);
}


void main(void){

    uint pIndex= gl_GlobalInvocationID.x;
    uint globalInvocY = gl_GlobalInvocationID.y;
    //pxm[pIndex].x +=0.0005;
    vec4 particle = pxm[pIndex];
    vec4 particleVelocity = pv[pIndex];

    vec3 xp= particle.xyz; //particle position
    float mp = particle.w; // particle mass
    vec3 vp = particleVelocity.xyz; //particle velocity
    //float Vp = particleVelocity.w; //particle Volume

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

        vec3 gridDistanceToParticle = ParticleInGrid- vec3(gridIndex);
        float wip = .0f;
        weighting (gridDistanceToParticle,wip);

        getIndex(gridIndex,gI);

        gxm[gI].w+=mp * wip; // calculate gridMass
        barrier();
        float mi = gxm[gI].w;
        pv[pIndex].w+= mp / (mi * wip / gCellVolume)  ; //calculate particleVolume

   }

}
