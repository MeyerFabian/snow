#version 440
uniform float dt;

uniform int collisionOffset;
uniform ivec3 gGridDim;
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
float n = 0.0f;
vec3 zeroVelocity = vec3(0.0f,0.0f,0.0f);
vec3 g = vec3(0.0f,-9.81f,0.0f);


int numCollisionObjects = 6;
/*
vec4 cpositions[6] = vec4[6](
vec4(gxm[collisionOffset*gGridDim[0].x*gGridDim[1].x+collisionOffset*gGridDim[0].x+collisionOffset]),
vec4(gxm[collisionOffset*gGridDim[0].x*gGridDim[1].x+collisionOffset*gGridDim[0].x+collisionOffset]),
vec4(gxm[collisionOffset*gGridDim[0].x*gGridDim[1].x+collisionOffset*gGridDim[0].x+collisionOffset]),
vec4(gxm[(gGridDim[2].x-collisionOffset)*gGridDim[0].x*gGridDim[1].x+(gGridDim[1].x-collisionOffset)*gGridDim[0].x+(gGridDim[0].x-collisionOffset)]),
vec4(gxm[(gGridDim[2].x-collisionOffset)*gGridDim[0].x*gGridDim[1].x+(gGridDim[1].x-collisionOffset)*gGridDim[0].x+(gGridDim[0].x-collisionOffset)]),
vec4(gxm[(gGridDim[2].x-collisionOffset)*gGridDim[0].x*gGridDim[1].x+(gGridDim[1].x-collisionOffset)*gGridDim[0].x+(gGridDim[0].x-collisionOffset)])
);
*/
int ctype[7] = int[7](
            0,
            0,
            0,
            0,
            0,
            0,
            1);
vec4 cpositions[7] = vec4[7](
    vec4(0.7125f),
    vec4(0.7125f),
    vec4(0.7125f),
    vec4(10.3625f),
    vec4(10.3625f),
    vec4(10.3625f),
    vec4(0.5f,2.125f,5.0f,1.0f)
);
vec4 cnormals[7] = vec4[7](
    vec4(1.0f,0.0f,0.0f,0.0f),
    vec4(0.0f,1.0f,0.0f,0.0f),
    vec4(0.0f,0.0f,1.0f,0.0f),
    vec4(-1.0f,0.0f,0.0f,0.0f),
    vec4(0.0f,-1.0f,0.0f,0.0f),
    vec4(0.0f,0.0f,-1.0f,0.0f),
    vec4(0.0f)
);
vec4 cvelocities[7] = vec4[7](
    vec4(0.0f,0.0f,0.0f,0.0f),
    vec4(0.0f,0.0f,0.0f,0.0f),
    vec4(0.0f,0.0f,0.0f,0.0f),
    vec4(0.0f,0.0f,0.0f,0.0f),
    vec4(0.0f,0.0f,0.0f,0.0f),
    vec4(0.0f,0.0f,0.0f,0.0f),
    vec4(10.0f,0.0f,0.0f,0.0f)
);
float cfriction[7] = float[7](0.1f,0.1f,0.1f,0.1f,0.1f,0.1f,0.1f);
bool collides(const vec3 pPos,const vec3 cPos,const vec3 cNormal){
    return dot((pPos-cPos),cNormal) <= 0;
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
        for(int i = 0 ; i<numCollisionObjects; i++){
            vec3 p = cpositions[i].xyz;
            vec3 n = cnormals[i].xyz;
            if(collides(xi,p,n)){
                vec3 vco = cvelocities[i].xyz;
                vec3 vrel = vin - vco;
                float vn = dot(vrel,n);
                if(vn<0){
                    vec3 vt = vrel - n*vn;
                    float muvn = cfriction[i] * vn;
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
