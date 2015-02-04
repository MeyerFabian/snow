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
    //rv[pI].xyz += dt * vec3(0.0f,20.0f,0.0f);

    rp[pI].xyz += dt *  rv[pI].xyz;
    if(uint( rp[pI].w) == 1){

       // rv[pI].xyz +=  vec3(0.0f,12.0f*dt,0.0f);
    }
    //rp[pI].w =0.0f;
}

