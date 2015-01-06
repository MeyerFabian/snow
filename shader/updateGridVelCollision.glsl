#version 440
#extension GL_ARB_compute_variable_group_size :require
#extension  NV_shader_atomic_float:require
uniform float dt;

layout(local_size_variable)in;

layout(std140, binding = 2) buffer gPosMass {
    vec4 gxm[ ];
};
layout(std140, binding = 3) buffer gVel {
    vec4 gv[ ];
};
layout(std140, binding = 6) buffer gForce{
    vec4 gf[] ;
};
layout(std140, binding = 7) buffer gVeln {
    vec4 gvn[ ];
};
float n = 0.0f;
vec3 zeroVelocity = vec3(0.0f,0.0f,0.0f);
vec3 g = vec3(0.0f,0.0f,0.0f);

//vec3 readVel; //2
void main(void){
    uint gI = gl_GlobalInvocationID.x;
    float mi = gxm[gI].w;
    vec3 vi =  gv[gI].xyz; //needs to be normalized with mi
    vec3 fi = gf[gI].xyz;//+vec3(0.0001,0.0002,0.0);
    //vin+1 = vin + d_t * mi^(-1) * fin

    if(mi>0.0f){


    gvn[gI].xyz =vi/mi + dt
             * (
               fi/mi+
                g)
            ;

    //fi[gI].xyz += force;

    }
    else{
        gvn[gI].xyz =zeroVelocity;
    }
}
