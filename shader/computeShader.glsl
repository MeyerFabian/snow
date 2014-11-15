#version 430

uniform vec3 gGridPos;

layout(std140, binding = 0) buffer pPosMass {
    vec4 pPositionsMass[ ];
};

layout(std140, binding = 1) buffer pVel {
    vec4 pVelocities[ ];
};

layout(std140, binding = 3) buffer gVel {
    vec4 gVelocities[ ];
};
layout(std140, binding = 4) buffer gMass {
    float gMasses[ ];
};


layout(local_size_x=1024,local_size_y=1,local_size_z=1)in;
void main(void){
    uint index = gl_GlobalInvocationID.x;
    pPositionsMass[index].x += 0.0015 ;
}
