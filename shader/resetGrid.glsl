#version 440
#extension GL_ARB_compute_variable_group_size :require


layout(local_size_variable)in;

layout(std140, binding = 2) buffer gPos {
    vec4 gPositionsMass[ ];
};
layout(std140, binding = 3) buffer gVel {
    ivec4 gVelocities[ ];
};
layout(std140, binding = 6) buffer gForce{
    ivec4 fi[] ;
};

float n = 0.0f;
vec3 zeroVelocity = vec3(0.0f,0.0f,0.0f);
//vec3 readVel; //2
void main(void){
    /*
        Testing if writing is more expensive than reading.
        1. only writing
        2. writing only if the value in there is not zero  (just slightly faster atm, error involved)
    */

    //readVel=gVelocities[gl_GlobalInvocationID.x].xyz; //2
    //if(gPositionsMass[gl_GlobalInvocationID.x].w > 1e-10) //2

    //if( readVel.x > 1e-10 || readVel.y > 1e-10 || readVel.z > 1e-10)//2
    gVelocities[gl_GlobalInvocationID.x] = ivec4(0,0,0,0);//1&2
    fi[gl_GlobalInvocationID.x].xyz = ivec3(0,0,0);
}
