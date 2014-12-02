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
    mat3 FEp[ ];
};
layout(std140, binding = 5) buffer pForcePlastic {
    mat3 FPp[ ];
};
layout(std140, binding = 6) buffer gForce{
    mat3 fi[] ;
};

int n= 0;



#define GAMMA 5.828427124 // FOUR_GAMMA_SQUARED = sqrt(8)+3;
#define CSTAR 0.923879532 // cos(pi/8)
#define SSTAR 0.3826834323 // sin(p/8)
void jacobiConjugation( int x, int y, int z, inout mat3 S, inout vec4 qV )
{
/*
    // eliminate off-diagonal entries Spq, Sqp
    float ch = 2.f * (S[0]-S[4]), ch2 = ch*ch;
      quaternion defines needed
    float sh = S[3], sh2 = sh*sh;
    bool flag = ( GAMMA * sh2 < ch2 );
    float w = rsqrtf( ch2 + sh2 );
    ch = flag ? w*ch : CSTAR; ch2 = ch*ch;
    sh = flag ? w*sh : SSTAR; sh2 = sh*sh;

    // build rotation matrix Q
    float scale = 1.f / (ch2 + sh2);
    float a = (ch2-sh2) * scale;
    float b = (2.f*sh*ch) * scale;
    float a2 = a*a, b2 = b*b, ab = a*b;

    // Use what we know about Q to simplify S = Q' * S * Q
    // and the re-arranging step.
    float s0 = a2*S[0] + 2*ab*S[1] + b2*S[4];
    float s2 = a*S[2] + b*S[5];
    float s3 = (a2-b2)*S[1] + ab*(S[4]-S[0]);
    float s4 = b2*S[0] - 2*ab*S[1] + a2*S[4];
    float s5 = a*S[7] - b*S[6];
    float s8 = S[8];
    S = mat3( s4, s5, s3,
              s5, s8, s2,
              s3, s2, s0 );

    vec3 tmp( sh*qV.x, sh*qV.y, sh*qV.z );
    sh *= qV.w;
    // original
    qV *= ch;

    qV[z] += sh;
    qV.w -= tmp[z];
    qV[x] += tmp[y];
    qV[y] -= tmp[x];
    */
}

void jacobiEigenanalysis(inout mat3 S,inout vec4 qV )
{
    qV = vec4( 1,0,0,0 );

    jacobiConjugation( 0, 1, 2, S, qV );
    jacobiConjugation( 1, 2, 0, S, qV );
    jacobiConjugation( 2, 0, 1, S, qV );

    jacobiConjugation( 0, 1, 2, S, qV );
    jacobiConjugation( 1, 2, 0, S, qV );
    jacobiConjugation( 2, 0, 1, S, qV );

    jacobiConjugation( 0, 1, 2, S, qV );
    jacobiConjugation( 1, 2, 0, S, qV );
    jacobiConjugation( 2, 0, 1, S, qV );

    jacobiConjugation( 0, 1, 2, S, qV );
    jacobiConjugation( 1, 2, 0, S, qV );
    jacobiConjugation( 2, 0, 1, S, qV );
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

    vec3 xp= particle.xyz; //particle position
    float mp = particle.w; // particle mass
    vec3 vp = particleVelocity.xyz; //particle velocity
    //float Vp = particleVelocity.w; //particle Volume


    int gridOffsetOfParticle = int(globalInvocY); //  597%64=21
    ivec3 gridOffset;

    getIJK(gridOffsetOfParticle,gridOffset ); // temp = 21%16 = 5, ijk=(5%4, 5/4, 21/16) = (1,1,1)+(-2,-2,-2) = (-1,-1,-1)

    vec3 ParticleInGrid= (xp- gGridPos)/gridSpacing;

    ivec3 gridIndex = ivec3(ParticleInGrid) + gridOffset;


    // gridIndex= ivec3(((pPositionsMass[9].xyz - (-1.05,0.05,-1.05))/0.1) + (-1,-1,-1)
    //          = ivec3(((0.4,0.5,-0.5)-(-1.05,0.05,-1.05))/0.1))+(-1,-1,-1)
    //          = (1.45,0.55,0.55)/0.1 +(-1,-1,-1)
    //          = (14.5,5.5,5.5) +(-1,-1,-1)
    //          = (13.5,4.5,4.5)
    if(gridIndex.x>= n && gridIndex.y>=n && gridIndex.z>=n && gridIndex.x< gGridDim[0].x && gridIndex.y <gGridDim[1].x &&gridIndex.z< gGridDim[2].x ){

        vec3 gridDistanceToParticle = ParticleInGrid- vec3(gridIndex);
        float wip = .0f;
        weighting (gridDistanceToParticle,wip);

        int gI;
        getIndex(gridIndex,gI);

        gxm[gI].w+= mp * wip; // calculate gridMasses

        barrier();
        float mi= gxm[gI].w;

        gv[gI].xyz+= vp * mp * wip / mi; // calculate gridVelocity
        vec4 quat;

   }

}
