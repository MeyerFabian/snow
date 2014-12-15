#version 440
#extension GL_ARB_compute_variable_group_size :require
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
vec3 g = vec3(0.0f,-9.81f,0.0f);
//vec3 readVel; //2
void main(void){
    uint gI = gl_GlobalInvocationID.x;
    float cellMass = gxm[gI].w;
    vec3 vi =  gv[gI].xyz;
    vec3 fi = gf[gI].xyz;
    //vin+1 = vin + d_t * mi^(-1) * fin
    if(cellMass>1e-12)
    gvn[gI].xyz =vi + dt
             * (fi/(cellMass) + g) ;

}
