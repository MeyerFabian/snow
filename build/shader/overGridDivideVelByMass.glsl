#version 440
#extension GL_ARB_compute_variable_group_size :require


layout(local_size_variable)in;
//try using y

layout(std140, binding = 2) buffer gPos {
    vec4 gPositionsMass[ ];
};
layout(std140, binding = 3) buffer gVel {
    vec4 gVelocities[ ];
};

void main(void){
    float gridPointMass = gPositionsMass[gl_GlobalInvocationID.x].w;
    gVelocities[gl_GlobalInvocationID.x].xyz /= gridPointMass;
}
