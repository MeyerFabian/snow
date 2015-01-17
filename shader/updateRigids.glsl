#version 440
uniform float dt;
layout(local_size_x =1, local_size_y =1,local_size_z =1)in;

layout(std140, binding = 8) buffer pPosMass {
    vec4 rp[ ];
};
layout(std140, binding = 9) buffer pVelVolume {
    vec4 rv[ ];
};
layout(std140, binding = 10) buffer pNormal {
    vec4 rn[ ];
};






void main(void){
    uint pI = gl_GlobalInvocationID.x;

    vec3 rv = rv[pI].xyz;

    rp[pI].xyz += dt *  rv;
    //rp[pI].w =0.0f;
}

