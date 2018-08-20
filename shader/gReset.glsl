#version 440
layout(local_size_x =1024, local_size_y =1,local_size_z =1)in;

layout(std140, binding = 3) buffer gVel {
	ivec4 gVelocities[ ];
};
layout(std140, binding = 16) buffer gForce {
	ivec4 gForces[ ];
};

void main(void){

	gVelocities[gl_GlobalInvocationID.x] = ivec4(0,0,0,0);
	gForces[gl_GlobalInvocationID.x].xyz = ivec3(0,0,0);

}
