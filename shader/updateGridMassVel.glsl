#version 440
uniform vec3 gGridPos;
uniform ivec3 gGridDim;
uniform float gridSpacing;
uniform float young;
uniform float poisson;
uniform float hardening;
uniform float critComp;
uniform float critStretch;



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



//QUATERNION MATH
struct quat{
    float data[4];
};

void multUpScalar ( inout vec4 q, const float s )
{ q[0] *= s; q[1] *= s; q[2] *= s; q[3] *= s;}
//quat operator * ( float f ) const { return quat( w*f, x*f, y*f, z*f ); }

//MATRIX MATH
vec3 column( int i ,const mat3 data) {
    //int j = 3*i;
    return vec3( data[i][0], data[i][1], data[i][2]);
    // return vec3( data[j], data[j+1], data[j+2] );
}


//* SVD IMPLEMENTATION SNOW CUDA *//

#define GAMMA 5.828427124 // FOUR_GAMMA_SQUARED = sqrt(8)+3;
#define CSTAR 0.923879532 // cos(pi/8)
#define SSTAR 0.3826834323 // sin(p/8)
#define EPSILON 1e-6
void fromQuat( const vec4 q, inout mat3 M )
{
    float qxx = q[1]*q[1];
    float qyy = q[2]*q[2];
    float qzz = q[3]*q[3];
    float qxz = q[1]*q[3];
    float qxy = q[1]*q[2];
    float qyz = q[2]*q[3];
    float qwx = q[0]*q[1];
    float qwy = q[0]*q[2];
    float qwz = q[0]*q[3];
    M[0][0] = 1.f - 2.f*(qyy+qzz);
    M[0][1] = 2.f * (qxy+qwz);
    M[0][2] = 2.f * (qxz-qwy);
    M[1][0] = 2.f * (qxy-qwz);
    M[1][1] = 1.f - 2.f*(qxx+qzz);
    M[1][2] = 2.f * (qyz+qwx);
    M[2][0] = 2.f * (qxz+qwy);
    M[2][1] = 2.f * (qyz-qwx);
    M[2][2] = 1.f - 2.f*(qxx+qyy);
}
/*
static mat3 fromQuat( const quat &q )
{
    float qxx = q.x*q.x;
    float qyy = q.y*q.y;
    float qzz = q.z*q.z;
    float qxz = q.x*q.z;
    float qxy = q.x*q.y;
    float qyz = q.y*q.z;
    float qwx = q.w*q.x;
    float qwy = q.w*q.y;
    float qwz = q.w*q.z;
    mat3 M;
    M[0] = 1.f - 2.f*(qyy+qzz);
    M[1] = 2.f * (qxy+qwz);
    M[2] = 2.f * (qxz-qwy);
    M[3] = 2.f * (qxy-qwz);
    M[4] = 1.f - 2.f*(qxx+qzz);
    M[5] = 2.f * (qyz+qwx);
    M[6] = 2.f * (qxz+qwy);
    M[7] = 2.f * (qyz-qwx);
    M[8] = 1.f - 2.f*(qxx+qyy);
    return M;
}
*/

void jacobiConjugation( int x, int y, int z, inout mat3 S, inout vec4 qV ,inout vec3 test)
{

 // eliminate off-diagonal entries Spq, Sqp
 float ch = 2.0f * ((S[0][0])-(S[1][1])), ch2 = ch*ch;
 //float ch = 2.f * (S[0]-S[4]), ch2 = ch*ch;
 float sh = S[0][1], sh2 = sh*sh;
 //float sh = S[3], sh2 = sh*sh;
 test.x = S[0][0]-S[1][1];
 test.y = sh;
 bool flag = ( GAMMA * sh2 < ch2 );

 float w = 1.0f/sqrt( ch2 + sh2 );
//float w = rsqrtf( ch2 + sh2 );

 ch = flag ? w*ch : CSTAR; ch2 = ch*ch;
 sh = flag ? w*sh : SSTAR; sh2 = sh*sh;

 // build rotation matrix Q
 float scale = 1.0f / (ch2 + sh2);
float a = (ch2-sh2) * scale;
 float b = (2.f*sh*ch) * scale;
 float a2 = a*a, b2 = b*b, ab = a*b;

 // Use what we know about Q to simplify S = Q' * S * Q
 // and the re-arranging step.
 float s0 = a2*S[0][0] + 2.0f*ab*S[1][0] + b2*S[1][1];
 //float s0 = a2*S[0] + 2*ab*S[1] + b2*S[4];
 float s2 = a*S[2][0] + b*S[2][1];
 //float s2 = a*S[2] + b*S[5];
 float s3 = (a2-b2)*S[1][0] + ab*(S[1][1]-S[0][0]);
 //float s3 = (a2-b2)*S[1] + ab*(S[4]-S[0]);
 float s4 = b2*S[0][0] - 2.0f*ab*S[1][0] + a2*S[1][1];
  //float s4 = b2*S[0] - 2*ab*S[1] + a2*S[4];
 float s5 = a*S[1][2] - b*S[0][2];
 //float s5 = a*S[7] - b*S[6];
 float s8 = S[2][2];
 //float s8 = S[8];

 S = mat3( s4, s5, s3,
 s5, s8, s2,
 s3, s2, s0 );
 vec3 tmp=vec3( sh * qV.y, sh*qV.z, sh*qV.w );
 // vec3 tmp( sh*qV.x, sh*qV.y, sh*qV.z );
 sh *= qV.x;
 //sh *= qV.w;

 // original
qV*=ch;
 //qV *= ch;

 qV[z+1] += sh;
 qV.x -=tmp[z];
 //qV.w -= tmp[z];
 qV[x+1] += tmp[y];
 qV[y+1] -= tmp[x];
//qV[0]=1; qV[1]=0; qV[2]=0; qV[3]=0;
}

void jacobiEigenanalysis(inout mat3 S,inout vec4 qV )
{
 qV[0]=1.0f; qV[1]=0.0f; qV[2]=0.0f; qV[3]=0.0f;// = quat( 1,0,0,0 );
vec3 tmp;
 jacobiConjugation( 0, 1, 2, S, qV ,tmp);
 jacobiConjugation( 1, 2, 0, S, qV ,tmp);
 jacobiConjugation( 2, 0, 1, S, qV ,tmp);


 jacobiConjugation( 0, 1, 2, S, qV ,tmp);
/*
 qV.x = tmp.x;
 qV.y =tmp.y;
 qV.z = 0.0f;
 qV.w =0.0f;
*/

 jacobiConjugation( 1, 2, 0, S, qV,tmp );
 jacobiConjugation( 2, 0, 1, S, qV ,tmp);

 jacobiConjugation( 0, 1, 2, S, qV ,tmp);
 jacobiConjugation( 1, 2, 0, S, qV,tmp );
 jacobiConjugation( 2, 0, 1, S, qV,tmp );

 jacobiConjugation( 0, 1, 2, S, qV ,tmp);
 jacobiConjugation( 1, 2, 0, S, qV ,tmp);
 jacobiConjugation( 2, 0, 1, S, qV,tmp );




 //qV[0]=1; qV[1]=0; qV[2]=0; qV[3]=0;// = quat( 1,0,0,0 );
}

/*
#define condSwap( COND, X, Y )          \
{                                       \
    __typeof__ (X) _X_ = X;             \
    X = COND ? Y : X;                   \
    Y = COND ? _X_ : Y;                 \
}

#define condNegSwap( COND, X, Y )       \
{                                       \
    __typeof__ (X) _X_ = -X;            \
    X = COND ? Y : X;                   \
    Y = COND ? _X_ : Y;                 \
}
*/
void condSwap(bool c,inout float x,inout float y){
    float temp = x;
    x= c?y:x;
    y= c?temp:y;
}

void condNegSwap(bool c, inout vec3 x,inout  vec3 y){
    vec3 temp = -x;
    x= c?y:x;
    y= c?temp:y;
}

void sortSingularValues(inout mat3 B,inout mat3 V )
{

    // used in step 2
    vec3 b1 = column(0,B); vec3 v1 = column(0,V);
    //vec3 b1 = B.column(0); vec3 v1 = V.column(0);
    vec3 b2 = column(1,B); vec3 v2 = column(1,V);
    //vec3 b2 = B.column(1); vec3 v2 = V.column(1);
    vec3 b3 = column(2,B); vec3 v3 = column(2,V);
    //vec3 b3 = B.column(2); vec3 v3 = V.column(2);

    float rho1 = dot( b1, b1 );
    float rho2 = dot( b2, b2 );
    float rho3 = dot( b3, b3 );
    bool c;

    c = rho1 < rho2;
    condNegSwap( c, b1, b2 );
    condNegSwap( c, v1, v2 );
    condSwap( c, rho1, rho2 );

    c = rho1 < rho3;
    condNegSwap( c, b1, b3 );
    condNegSwap( c, v1, v3 );
    condSwap( c, rho1, rho3 );

    c = rho2 < rho3;
    condNegSwap( c, b2, b3 );
    condNegSwap( c, v2, v3 );

    // re-build B,V
    B = mat3( b1,b2,b3);
    V = mat3( v1,v2,v3);




}


 void QRGivensQuaternion( float a1, float a2,inout float ch,inout float sh )
{
    // a1 = pivot point on diagonal
    // a2 = lower triangular entry we want to annihilate

    float rho = sqrt( a1*a1 + a2*a2 );
    //float rho = sqrtf( a1*a1 + a2*a2 );

    sh = rho > EPSILON ? a2 : 0;
    ch = abs(a1) + max( rho, EPSILON );
    //ch = fabsf(a1) + fmaxf( rho, EPSILON );
    bool b = a1 < 0;
    condSwap( b, sh, ch );
    float w = inversesqrt( ch*ch + sh*sh );
    //float w = rsqrtf( ch*ch + sh*sh );
    ch *= w;
    sh *= w;
}




 void QRDecomposition( const mat3 B, inout mat3 Q,inout mat3 R )
 {

     R = B;

     // QR decomposition of 3x3 matrices using Givens rotations to
     // eliminate elements B21, B31, B32
     vec4 qQ; // cumulative rotation
     mat3 U;
     float ch, sh, s0, s1;

     // first givens rotation
     QRGivensQuaternion( R[0][0], R[0][1], ch, sh );
     //QRGivensQuaternion( R[0], R[1], ch, sh );

     s0 = 1.0f-2.0f*sh*sh;
     s1 = 2.0f*sh*ch;
     U = mat3(  s0, s1, 0.0,
               -s1, s0, 0.0,
                 0,  0, 1.0 );
     R= transpose(U)*R;
     //R = mat3::multiplyAtB( U, R );

     qQ[0]=1.0; qQ[1]=0.0; qQ[2]=0.0; qQ[3]=0.0;

     // update cumulative rotation
     float q0 = ch*qQ[0]-sh*qQ[3];
     float q1 = ch*qQ[1]+sh*qQ[2];
     float q2 = ch*qQ[2]-sh*qQ[1];
     float q3 = sh*qQ[0]+ch*qQ[3];
     qQ[0]=q0; qQ[1]=q1; qQ[2]=q2; qQ[3]=q3;
     //qQ = quat( ch*qQ.w-sh*qQ.z, ch*qQ.x+sh*qQ.y, ch*qQ.y-sh*qQ.x, sh*qQ.w+ch*qQ.z );

     // second givens rotation
     QRGivensQuaternion( R[0][0], R[0][2], ch, sh );
     //QRGivensQuaternion( R[0], R[2], ch, sh );

     s0 = 1.0f-2.0f*sh*sh;
     s1 = 2.0f*sh*ch;
     U = mat3(  s0, 0.0f, s1,
                 0.0f, 1.0f,  0.0f,
               -s1, 0.0f, s0 );

     R= transpose(U)*R;
     //R = mat3::multiplyAtB( U, R );

     // update cumulative rotation
     q0 = ch*qQ[0]+sh*qQ[2];
     q1 = ch*qQ[1]+sh*qQ[3];
     q2 = ch*qQ[2]-sh*qQ[0];
     q3 = ch*qQ[3]-sh*qQ[1];
     qQ[0]=q0; qQ[1]=q1; qQ[2]=q2; qQ[3]=q3;
     //qQ = quat( ch*qQ.w+sh*qQ.y, ch*qQ.x+sh*qQ.z, ch*qQ.y-sh*qQ.w, ch*qQ.z-sh*qQ.x );

     // third Givens rotation
     QRGivensQuaternion( R[1][1], R[1][2], ch, sh );
     //QRGivensQuaternion( R[4], R[5], ch, sh );

     s0 = 1.0f-2.0f*sh*sh;
     s1 = 2.0f*sh*ch;
     U = mat3( 1.0f,   0.0f,  0.0f,
               0.0f,  s0, s1,
               0.0f, -s1, s0 );
     R= transpose(U)*R;
     //R = mat3::multiplyAtB( U, R );

     // update cumulative rotation
     q0 =  ch*qQ[0]-sh*qQ[1];
     q1 = sh*qQ[0]+ch*qQ[1];
     q2 = ch*qQ[2]+sh*qQ[3];
     q3 = ch*qQ[3]-sh*qQ[2];
     qQ[0]=q0; qQ[1]=q1; qQ[2]=q2; qQ[3]=q3;
     //qQ = quat( ch*qQ.w-sh*qQ.x, sh*qQ.w+ch*qQ.x, ch*qQ.y+sh*qQ.z, ch*qQ.z-sh*qQ.y );
    //qQ[0] =0.0 ;  qQ[1] = 1/sqrt(2);  qQ[2] = 0.0;  qQ[3] = 1/(sqrt(2));
     // qQ now contains final rotation for Q
     fromQuat(qQ,Q);



 }

void computeSVD( const mat3 A,inout mat3 W,inout mat3 S,inout mat3 V )
{
    // normal equations matrix
    mat3 AT = transpose(A);
    mat3 ATA = AT * A ;
/// 2. Symmetric Eigenanlysis
    vec4 qV;
    jacobiEigenanalysis( ATA, qV );


/*
    V[0][0] = qV.x;
    V[0][1] = qV.y;
    V[0][2] = qV.z;
    V[1][0] = qV.w;
*/
//V= ATA;

    fromQuat(qV,V);

    //V = mat3::fromQuat(qV);
    mat3 B =A*V;


/// 3. Sorting the singular values (find V)
    sortSingularValues( B, V );

    //V=B;
/// 4. QR decomposition
    QRDecomposition( B, W, S );


}


void computePD( const mat3 A, inout mat3 R, inout mat3 P )
{
    // U is unitary matrix (i.e. orthogonal/orthonormal)
    // P is positive semidefinite Hermitian matrix
    mat3 W, S, V;
    computeSVD( A, W, S, V );


    R = W*transpose(V);

    P = V*S*transpose(V);
}
//* SVD IMPLEMENTATION SNOW CUDA END*//

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

    //pxm[pIndex].x +=0.00005;

    vec4 particle = pxm[pIndex];
    vec4 particleVelocity = vec4(pv[pIndex]);
    mat4 FEp4 = mat4(pFE[pIndex]);
    mat3 FEp =mat3(FEp4);
    mat4 FPp4 = mat4(pFP[pIndex]);
    mat3 FPp = mat3(FPp4);

    vec3 xp= particle.xyz; //particle position
    float mp = particle.w; // particle mass
    vec3 vp = particleVelocity.xyz; //particle velocity
    float pp0 = float(particleVelocity.w); //particle density

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
        atomicAdd(gv[2*gI].w,  int(mp * wip* 1000000.0f));

        //vin = sum_p [ vpn * mp *wipn / min]
        //gv[gI].xyz+= vp * mp * wip; // calculate added gridVelocity

        vec3 velocity = (vp * mp * wip);
        atomicAdd(gv[2*gI].x,int(velocity.x));
        atomicAdd(gv[2*gI].y,int(velocity.y));
        atomicAdd(gv[2*gI].z,int(velocity.z));


        mat3 REp, SEp;
        computePD(FEp,REp,SEp);
        for(int i=0; i<3; i++){
            for(int j=0;j<3;j++){
                REp[i][j] =round(100000.0f *REp[i][j])/100000.0f ;
            }
        }
        float JPp = determinant(FPp);
        float JEp = determinant(FEp);
        vec3 wipg;
        weightingGradient(gridDistanceToParticle,wipg);
        // fi(^x) = - sum_p [ Vpn * sigmaP * d_wipn]
        //        = - sum_p [ Vp0 * (Jpn * 2 * mu(FPp)/Jpn * (FEp-REp) * FEp^(T) + Jpn* lamba(FPp)/Jpn* (JEp -1.0f) * JEp * FEp^(-T) * FEp^(T))*d_wipn]
        //        = - sum_p [ Vp0  * (2 * mu(FPp) * (FEp-REp) * FEp^(T) + lamba(FPp)* (JEp -1.0f) * JEp * I )*d_wipn]
        if(pp0>0.0f){
           // if(gridOffset ==0 &&gridOffset ==0 &&gridOffset ==0){
        vec3 force = -

                (mp/pp0)*

                (  2.0f* mu(JPp)*
                             (FEp-REp)*transpose(FEp)
                    + lambda(JPp)*(JEp -1.0f)*(JEp)* mat3(1.0f)
                    )

                  *

        wipg;
        //force = wipg;
        //fi[gI].xyz += force;
        atomicAdd(gv[2*gI+1].x,int(force.x*1000000.0f));
        atomicAdd(gv[2*gI+1].y,int(force.y*1000000.0f));
        atomicAdd(gv[2*gI+1].z,int(force.z*1000000.0f));

        }
   }

}
