#version 440

uniform vec3 gGridPos;
uniform ivec3 gGridDim;
uniform float dt;
uniform float gridSpacing;

layout(std140, binding = 0) buffer pPosMass {
    vec4 pPositionsMass[ ];
};

layout(std140, binding = 1) buffer gPos {
    vec4 gPositionsMass[ ];
};

layout(std140, binding = 2) buffer pVel {
    vec4 pVelocities[ ];
};

layout(std140, binding = 3) buffer gVel {
    vec4 gVelocities[ ];
};


layout(local_size_x= 1024, local_size_y =1, local_size_z = 1)in;

int width = 4;
uint threadNum = 64;

void getIndex(inout ivec3 ijk,inout int index){
    index = ijk.x + (ijk.y * int(gGridDim[0].x)) + (ijk.z *int(gGridDim[1].x) * int(gGridDim[0].x));
}
void getIJK(inout ivec3 ijk, inout int index){
    int temp = index%(gGridDim[0].x*gGridDim[1].x);
    ijk= ivec3(temp%gGridDim[0].x,temp/gGridDim[0].x,index/(gGridDim[0].x*gGridDim[1].x));
}

void main(void){
    uint globalInvoc = gl_GlobalInvocationID.x;

    pPositionsMass[globalInvoc/threadNum].x +=0.0005;

    int gridOffsetOfParticle = int(globalInvoc%threadNum);
    ivec3 gridOffset;

    getIJK(gridOffset,gridOffsetOfParticle );

    ivec3 gridIndex = ivec3((pPositionsMass[globalInvoc/threadNum].xyz- gGridPos)/gridSpacing) + gridOffset;
    int gPositionsMassIndex = 0;
    getIndex(gridIndex,gPositionsMassIndex);

    gPositionsMass[gPositionsMassIndex].w+=pPositionsMass[globalInvoc/threadNum].w;
}
