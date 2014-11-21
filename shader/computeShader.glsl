#version 440
#define GL_ARB_compute_variable_group_size 1
uniform vec3 gGridPos;
uniform ivec3 gGridDim;
uniform float dt;
uniform float gridSpacing;

layout(local_size_x=1024)in;

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



void getIndex(inout int i , inout int j ,inout int k, inout int index){
    index = i + (j * int(gGridDim[0].x)) + (k *int(gGridDim[1].x) * int(gGridDim[0].x));
}

void main(void){

    uint index = gl_GlobalInvocationID.x; //index = 9
    vec3 ParticleInGrid = (pPositionsMass[index].xyz- gGridPos);
    // ParticleInGrid   = (0.4,0.5,-0.5)-(-1.05,0.05,-1.05)
    //                  = (1.45,0.55,0.55)
    float ParticleMass = pPositionsMass[index].w;

int gIndex;
int gridIndX;
int gridIndY;
int gridIndZ;
int n= 0;

    for( int i=-1; i<=2 ; i++){
    gridIndX =int(ParticleInGrid.x/gridSpacing) +i; // i=-1
    // gridIndX = 1.45/0.1 - 1 = 14.5 -1 = 13.5

        for(int j=-1; j<=2; j++){
            gridIndY =int(ParticleInGrid.y/gridSpacing) +j; //j=-1
            //gridIndY = 0.55/0.1 - 1 = 5.5 -1 = 4.5
            for(int k=-1 ;k<=2;k++ ){
                gridIndZ =int(ParticleInGrid.z/gridSpacing) +k; //k=-1
                //gridIndY = 0.55/0.1 - 1 = 5.5 -1 = 4.5
                if(gridIndX>= n && gridIndY>=n && gridIndZ>=n  &&  gridIndX< gGridDim[0].x &&  gridIndY <gGridDim[1].x   &&  gridIndZ< gGridDim[2].x ){
                    getIndex(gridIndX,gridIndY,gridIndZ,gIndex);
                    //gIndex = 13+ 4*101+4*101*101 =41221
                    gPositionsMass[gIndex].w+=ParticleMass;
                    barrier();
                }
            }
        }
    }



    for( int i=-1; i<=2 ; i++){
    gridIndX =int(ParticleInGrid.x/gridSpacing) +i;

        for(int j=-1; j<=2; j++){
            gridIndY =int(ParticleInGrid.y/gridSpacing) +j;

            for(int k=-1 ;k<=2;k++ ){
                gridIndZ =int(ParticleInGrid.z/gridSpacing) +k;

                if(gridIndX>= n && gridIndY>=n && gridIndZ>=n  &&  gridIndX< gGridDim[0].x &&  gridIndY <gGridDim[1].x   &&  gridIndZ< gGridDim[2].x ){
                    getIndex(gridIndX,gridIndY,gridIndZ,gIndex);
                    gVelocities[gIndex].xyz+=pVelocities[index].xyz;
                    barrier();
                }
            }
        }
    }

}
