#version 440
#define alpha 0.95

uniform float dt;
uniform float critComp;
uniform float critStretch;
uniform uint gNumColliders;
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
layout(std140, binding = 4) buffer pForceElastic {
	mat4 pFE[ ];
};
layout(std140, binding = 5) buffer pForcePlastic {
	mat4 pFP[ ];
};
layout(std140, binding = 6) buffer pVeln {
	ivec4 pvn[ ];
};

layout(std140, binding = 8) buffer cPos {
	vec4 cx[ ];
};
layout(std140, binding = 9) buffer cVel {
	vec4 cv[ ];
};
layout(std140, binding = 10) buffer cNor {
	vec4 cn[ ];
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

#include "shader/compute/svd/SVD.include.glsl"

float n = 0.0f;
vec3 zeroVelocity = vec3(0.0f,0.0f,0.0f);

void sclamp (inout float f, const float lb, const float ub){
	f = (f<lb)? lb : (f>ub)? ub : f;
}

bool collidesHalfPlane(const vec3 pPos,const int i){
	return dot((pPos-cx[i].xyz),cn[i].xyz) <= 0;
}
bool collidesSphere(const vec3 pPos,const int i, inout vec3 n){
	float radius = cv[i].w;
	n = normalize(pPos-cx[i].xyz);
	return length(pPos-cx[i].xyz) < radius;
}

bool collides(const vec3 pPos,const int i, inout vec3 n){
	return (uint( cx[i].w) ==0)? collidesHalfPlane(pPos,i)
		:(uint(cx[i].w) ==1)?collidesSphere(pPos,i,n)
		:false;
}

void main(void){
	uint pI = gl_GlobalInvocationID.x;

	if (pI >= indexSize)
		return;

	// UPDATE DEFORMATION GRADIENT
	mat4 FEp4 = mat4(pFE[pI]);
	mat3 FEp = mat3(FEp4);
	mat4 FPp4 = mat4(pFP[pI]);
	mat3 FPp = mat3(FPp4);

	mat3 dvp = mat3( vec3(pdvp0[pI])*1e-8f,vec3(pdvp1[pI])*1e-8f,vec3(pdvp2[pI])*1e-8f);
	/*
	   for(int i=0; i<3; i++){
	   for(int j=0;j<3;j++){
	   dvp[i][j] =dvp[i][j]*10000.0f ;
	   }
	   }
	 */
	mat3 FEpn = (mat3(1.0f) + dt * dvp)*FEp;
	mat3 Fpn = (mat3(1.0f) + dt * dvp)* (FEp*FPp);
	mat3 FPpn = FPp;
	//FEpn = mat3(1.0025f);
	/*
	   for(int i=0; i<3; i++){
	   for(int j=0;j<3;j++){
	   Fpn[i][j] =round(1e5f*Fpn[i][j])/1e5f ;
	   FEpn[i][j] =round(1e5f *FEpn[i][j])/1e5f ;
	   }
	   }
	 */
	//FEpn= mat3(1.0f);
	//FEpn= mat3(2.0f,1.0f,1.0f,0.0f,3.0f,1.0f,2.0f,1.4f,2.4f);
	mat3 W =mat3(0.0f);
	mat3 S =mat3(0.0f);
	mat3 V =mat3(0.0f);
	computeSVD(FEpn,W,S,V);

	sclamp(S[0][0], 1.0f-critComp,1.0f+critStretch);
	sclamp(S[1][1], 1.0f-critComp,1.0f+critStretch);
	sclamp(S[2][2], 1.0f-critComp,1.0f+critStretch);

	FEpn = W*S*transpose(V);

	for(int i=0; i<3; i++){
		for(int j=0;j<3;j++){
			FEpn[i][j] =round(1e9f *FEpn[i][j])/1e9f ;
		}
	}


	mat3 S_I = mat3(0.0f);

	S_I[0][0]= 1.0f/S[0][0];
	S_I[1][1]= 1.0f/S[1][1];
	S_I[2][2]= 1.0f/S[2][2];
	FPpn =V   * S_I * transpose(W) *Fpn;

	//FPpn = inverse(FEpn) *Fpn;

	for(int i=0; i<3; i++){
		for(int j=0;j<3;j++){
			FPpn[i][j] =round(1e9f *FPpn[i][j])/1e9f ;
		}
	}

	/*
	   pFE[gl_GlobalInvocationID.x][0].xyz =vec3(0.0f,0.0f,1.0f);
	   pFE[gl_GlobalInvocationID.x][1].xyz =vec3(0.0f,1.0f,0.0f);
	   pFE[gl_GlobalInvocationID.x][2].xyz =vec3(1.0f,0.0f,0.0f);

	 */
	/*
	   pFE[gl_GlobalInvocationID.x] = mat4(1.0,0.0,0.0,0.0f,
	   0.0,1.0,0.0,0.0f,
	   0.0,0.0,1.0,0.0f,
	   0.0f,0.0f,0.0f,0.0f);


	 */

	pFE[gl_GlobalInvocationID.x] = mat4( FEpn);


	pFP[gl_GlobalInvocationID.x] = mat4( FPpn);


	/*
	   pFP[gl_GlobalInvocationID.x] = mat4( FPpn[0][0],FPpn[0][1],FPpn[0][2],0.0f,
	   FPpn[1][0],FPpn[1][1],FPpn[1][2],0.0f,
	   FPpn[2][0],FPpn[2][1],FPpn[2][2],0.0f,
	   0.0f,0.0f,0.0f,1.0);

	   pFE[gl_GlobalInvocationID.x][0].xyz =column(0,FEpn);
	   pFE[gl_GlobalInvocationID.x][1].xyz =column(1,FEpn);
	   pFE[gl_GlobalInvocationID.x][2].xyz =column(2,FEpn);
	 */
	// UPDATE VELOCITIES

	vec3 vpn = vec3(pvn[pI].xyz)*1e-8f;
	vec3 vp = vec3(pv[pI].xyz)*1e-8f;
	//vpn+1 = a * vpn + temp_vpn+1


	vec3 vpn1 = vp * alpha + vpn;

	for(int i = 0 ; i<gNumColliders; i++){
		vec3 p = cx[i].xyz;
		vec3 n = cn[i].xyz;
		vec3 particlePos = pxm[pI].xyz;
		if(collides(particlePos,i,n)){
			vec3 vco = cv[i].xyz;
			vec3 vrel = vpn1 - vco;
			float vn = dot(vrel,n);
			if(vn<0.0f){
				vec3 vt = vrel - n*vn;
				float muvn = cn[i].w * vn;
				vec3 vrelt=vt;
				float lengthvt=length(vt);
				if(lengthvt<= - muvn){
					vrelt = vec3(0.0f);
				}
				else{
					vrelt+=  muvn*vt/(lengthvt);
				}
				vpn1 = vrelt + vco;
			}
		}
	}

	pv[pI].xyz = ivec3(vpn1*1e8f);
	// UPDATE POSITION
	// xpn+1 = xpn + d_t * vpn+1

	pxm[pI].xyz += dt *  vpn1;

	//Reset vpn+1 and delta vpn+1 to (0,0,0)
	pvn[pI].xyz = ivec3(0,0,0);
	pdvp0[pI].xyz = ivec3(0,0,0);
	pdvp1[pI].xyz = ivec3(0,0,0);
	pdvp2[pI].xyz = ivec3(0,0,0);
}
