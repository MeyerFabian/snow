#version 440

uniform vec3 gGridPos;
uniform ivec3 gGridDim;
uniform float dt;
uniform float gridSpacing;

layout(std140, binding = 0) buffer pPosMass {
    vec4 pPositionsMass[ ];
};

layout(std140, binding = 1) buffer pVel {
    vec4 pVelocities[ ];
};

layout(std140, binding = 2) buffer gPos {
    vec4 gPositionsMass[ ];
};
layout(std140, binding = 3) buffer gVel {
    vec4 gVelocities[ ];
};

//try using y
layout(local_size_x= 1024, local_size_y =1, local_size_z = 1)in;

int width = 4;
ivec3 windowOffset=ivec3(-2,-2,-2);

int n= 0;
void getIndex(inout ivec3 ijk,inout int index){
    index = ijk.x + (ijk.y * int(gGridDim[0].x)) + (ijk.z *int(gGridDim[1].x) * int(gGridDim[0].x));
}
void getIJK(inout ivec3 ijk, inout int index){
    int temp = index%(width*width);
    ijk= ivec3(temp%width,temp/width,index/(width*width))+windowOffset;
}

void main(void){
    uint globalInvocX = gl_GlobalInvocationID.x;
    uint globalInvocY = gl_GlobalInvocationID.y;
    uint pIndex = globalInvocX;
    //pPositionsMass[pIndex].x +=0.0005;

    int gridOffsetOfParticle = int(globalInvocY); //  597%64=21
    ivec3 gridOffset;

    getIJK(gridOffset,gridOffsetOfParticle ); // temp = 21%16 = 5, ijk=(5%4, 5/4, 21/16) = (1,1,1)+(-2,-2,-2) = (-1,-1,-1)

    ivec3 gridIndex = ivec3((pPositionsMass[pIndex].xyz- gGridPos)/gridSpacing) + gridOffset;
    // gridIndex= ivec3(((pPositionsMass[9].xyz - (-1.05,0.05,-1.05))/0.1) + (-1,-1,-1)
    //          = ivec3(((0.4,0.5,-0.5)-(-1.05,0.05,-1.05))/0.1))+(-1,-1,-1)
    //          = (1.45,0.55,0.55)/0.1 +(-1,-1,-1)
    //          = (14.5,5.5,5.5) +(-1,-1,-1)
    //          = (13.5,4.5,4.5)
    if(gridIndex.x>= n && gridIndex.y>=n && gridIndex.z>=n && gridIndex.x< gGridDim[0].x && gridIndex.y <gGridDim[1].x &&gridIndex.z< gGridDim[2].x ){


        int gPositionsMassIndex = 0;
        getIndex(gridIndex,gPositionsMassIndex);
    // gPositionsMassIndex = 13+ 4*100+4*100*100 = 40413
        gPositionsMass[gPositionsMassIndex].w+=pPositionsMass[pIndex].w; // 87 Fps
        barrier();
        //gVelocities[gPositionsMassIndex].xyz+=pVelocities[pIndex].xyz; //27 Fps

   }
}
