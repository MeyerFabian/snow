#version 440
#extension GL_ARB_compute_variable_group_size :require
#define alpha 0.95
uniform float dt;

layout(local_size_variable)in;
//try using y

layout(std140, binding = 0) buffer pPosMass {
    vec4 pxm[ ];
};
layout(std140, binding = 1) buffer pVelVolume {
    vec4 pv[ ];
};
layout(std140, binding = 8) buffer pVeln {
    vec4 pvn[ ];
};
float n = 0.0f;
vec3 zeroVelocity = vec3(0.0f,0.0f,0.0f);
//vec3 readVel; //2
void main(void){
    uint pI = gl_GlobalInvocationID.x;

    vec3 vpn = pvn[pI].xyz;
    vec3 vp = pv[pI].xyz;
    pv[pI].xyz = vp * alpha + vpn;

    vpn = pv[pI].xyz;
    pxm[pI].xyz += dt * vpn;

    pvn[pI].xyz = zeroVelocity;
}
