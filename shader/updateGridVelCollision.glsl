#version 440
uniform float dt;

uniform int collisionOffset;
uniform ivec3 gGridDim;
uniform int gNumColliders;
layout(local_size_x =1024, local_size_y =1,local_size_z =1)in;

layout(std140, binding = 2) buffer gPosMass {
    vec4 gxm[ ];
};
layout(std140, binding = 3) buffer gVel {
    ivec4 gv[ ];
};

layout(std140, binding = 7) buffer gVeln {
    vec4 gvn[ ];
};

layout(std140, binding = 8) buffer cPos {
    vec4 cx[ ];
};
layout(std140, binding = 9) buffer cVel {
    vec4 cv[ ];
};
layout(std140, binding = 10) buffer cNor {
    vec4 cn[ ];
};
layout(std140, binding = 11) buffer cType {
    int ct[ ];
};
layout(std140, binding = 12) buffer cFric {
    float cf[ ];
};

float n = 0.0f;
vec3 zeroVelocity = vec3(0.0f,0.0f,0.0f);
vec3 g = vec3(0.0f,-9.81f,0.0f);
bool collidesHalfPlane(const vec3 pPos,const int i){
    return dot((pPos-cx[i].xyz),cn[i].xyz) <= 0;
}
bool collidesSphere(const vec3 pPos,const int i, inout vec3 n){
    float radius = cv[i].w;
    n = normalize(pPos-cx[i].xyz);
    return length(pPos-cx[i].xyz) < radius;
}

bool collides(const vec3 pPos,const int i, inout vec3 n){
    return (ct[i] ==0)? collidesHalfPlane(pPos,i)
           :(ct[i] ==1)?collidesSphere(pPos,i,n)
                      :false;
}


//vec3 readVel; //2
void main(void){
    uint gI = gl_GlobalInvocationID.x;
    vec3 xi = gxm[gI].xyz;
    float mi = float(gv[2*gI].w)/1000000.0f;
    vec3 vi =  vec3(gv[2*gI].xyz)/1000000.0f; //needs to be normalized with mi
    vec3 fi = vec3(gv[2*gI+1].xyz)/1000000.0f;//+vec3(0.0001,0.0002,0.0);
    //vin+1 = vin + d_t * mi^(-1) * fin

    if(mi>0.0f){
        vec3 vin =vi/mi + dt
            * (
            fi/mi+
            g)
        ;
        for(int i = 0 ; i<gNumColliders; i++){
            vec3 p = cx[i].xyz;
            vec3 n = cn[i].xyz;
            if(collides(xi,i,n)){
                vec3 vco = cv[i].xyz;
                vec3 vrel = vin - vco;
                float vn = dot(vrel,n);
                if(vn<0){
                    vec3 vt = vrel - n*vn;
                    float muvn = cf[i] * vn;
                    vec3 vrelt;
                    float lengthvt=length(vt);
                    if(lengthvt<= - muvn){
                        vrelt = vec3(0.0f);
                    }
                    else{
                        vrelt = vt + muvn*vt/(lengthvt);
                    }
                vin = vrelt + vco;
                }
            }
        }
        gvn[gI].xyz = vin;
            ;

    //fi[gI].xyz += force;

    }
    else{
        gvn[gI].xyz =zeroVelocity;
    }
}
