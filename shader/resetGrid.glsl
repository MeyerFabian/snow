#version 440


layout(local_size_x =1024, local_size_y =1,local_size_z =1)in;

layout(std140, binding = 2) buffer gPos {
    vec4 gPositionsMass[ ];
};
layout(std140, binding = 3) buffer gVel {
    ivec4 gVelocities[ ];
};
layout(std140, binding = 16) buffer gForce {
    ivec4 gForces[ ];
};

float n = 0.0f;
vec3 zeroVelocity = vec3(0.0f,0.0f,0.0f);
void main(void){
    /*
        Testing if writing is more expensive than reading.
        1. only writing
        2. writing only if the value in there is not zero  (just slightly faster atm, error involved)
    */
 //2
    //if(gPositionsMass[gl_GlobalInvocationID.x].w > 1e-10) //2

    gVelocities[gl_GlobalInvocationID.x] = ivec4(0,0,0,0);//1&2

    gForces[gl_GlobalInvocationID.x].xyz = ivec3(0,0,0);

}
