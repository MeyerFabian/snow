#version 440
#extension GL_ARB_compute_variable_group_size :require
uniform float dt;

layout(local_size_variable)in;

layout(std140, binding = 2) buffer gPosMass {
    vec4 gxm[ ];
};
layout(std140, binding = 3) buffer gVel {
    ivec4 gv[ ];
};
layout(std140, binding = 6) buffer gForce{
    ivec4 gf[] ;
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
    float mi = float(gv[gI].w)/1000000.0f;
    vec3 vi =  vec3(gv[gI].xyz)/1000000.0f; //needs to be normalized with mi
    vec3 fi = vec3(gf[gI].xyz)/1000000.0f;//+vec3(0.0001,0.0002,0.0);
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
