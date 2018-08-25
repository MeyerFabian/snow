#version 440
layout(local_size_x =1, local_size_y =1,local_size_z =1)in;
struct BufferData{
    mat4 A;
	mat4 U;
	mat4 S;
	mat4 V;
	mat4 R;
	mat4 P;
};

layout(std140, binding = 1) buffer matrices{
	BufferData data[ ];
};


#include "shader/compute/svd/SVD.include.glsl"

void main(void){
	uint i= gl_GlobalInvocationID.x;
	mat3 A = mat3(data[i].A);
	mat3 U,S,V,R,P;
	computeSVD(A,U,S,V);
	computePD(A,R,P);
	data[i].A = mat4(U*S*transpose(V));
	data[i].U =mat4(U);
	data[i].S =mat4(S);
	data[i].V =mat4(V);
	data[i].R =mat4(R);
	data[i].P =mat4(P);
}

