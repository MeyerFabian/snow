#version 440
#extension GL_ARB_compute_variable_group_size :require
#define alpha 0.95
uniform float dt;

layout(local_size_variable)in;

layout(std140, binding = 0) buffer pPosMass {
    vec4 pxm[ ];
};
layout(std140, binding = 1) buffer pVelVolume {
    vec4 pv[ ];
};
layout(std140, binding = 4) buffer pForceElastic {
    mat4 pFE[ ];
};
layout(std140, binding = 5) buffer pForcePlastic {
    mat4 pFP[ ];
};
layout(std140, binding = 8) buffer pVeln {
    vec4 pvn[ ];
};

layout(std140, binding = 9) buffer pDeltaVeln {
    vec4 deltapvn[ ];
};
float n = 0.0f;
vec3 zeroVelocity = vec3(0.0f,0.0f,0.0f);

void main(void){
    uint pI = gl_GlobalInvocationID.x;
    // UPDATE DEFORMATION GRADIENT
    mat4 FEp4 = mat4(pFE[pI]);
    mat3 FEp = mat3(FEp4[0].xyz,FEp4[1].xyz,FEp4[2].xyz);
    mat4 FPp4 = mat4(pFP[pI]);
    mat3 FPp =mat3(FPp4[0].xyz,FPp4[1].xyz,FPp4[2].xyz);

    // UPDATE VELOCITIES
    vec3 vpn = pvn[pI].xyz;
    vec3 vp = pv[pI].xyz;
    //vpn+1 = a * vpn + temp_vpn+1
    pv[pI].xyz = vp * alpha + vpn;

    // UPDATE POSITION
    vpn = pv[pI].xyz;
    // xpn+1 = xpn + d_t * vpn+1
    pxm[pI].xyz += dt * vpn;

    //Reset vpn+1 and delta vpn+1 to (0,0,0)
    pvn[pI].xyz = zeroVelocity;
    deltapvn[pI].xyz = zeroVelocity;
}
