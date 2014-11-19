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

layout(std140, binding = 1) buffer gPos {
    vec4 gPositionsMass[ ];
};

layout(std140, binding = 1) buffer pVel {
    vec4 pVelocities[ ];
};

layout(std140, binding = 4) buffer gVel {
    vec4 gVelocities[ ];
};



int getIndex(int i ,  int j , int k){
    return i + (j * int(gGridDim[0].x)) + (k *int(gGridDim[1].x) * int(gGridDim[0].x));
}

void main(void){

    uint index = gl_GlobalInvocationID.x;
    vec3 ParticleInGrid = (pPositionsMass[index].xyz- gGridPos);
    float ParticleMass = pPositionsMass[index].w;

    pPositionsMass[index].x +=0.0005;
int gIndex;
int gridIndX;
int gridIndY;
int gridIndZ;
int n= 0;

    for( int i=-1; i<=2 ; i++){
    gridIndX =int(ParticleInGrid.x/gridSpacing) +i;

        for(int j=-1; j<=2; j++){
            gridIndY =int(ParticleInGrid.y/gridSpacing) +j;

            for(int k=-1 ;k<=2;k++ ){
                gridIndZ =int(ParticleInGrid.z/gridSpacing) +k;

                if(gridIndX>= n && gridIndY>=n && gridIndZ>=n  &&  gridIndX< gGridDim[0].x &&  gridIndY <gGridDim[1].x   &&  gridIndZ< gGridDim[2].x ){
                    gIndex = getIndex(gridIndX,gridIndY,gridIndZ);
                    gPositionsMass[gIndex].w+=ParticleMass;
                }
            }
        }
    }

   //220FPS

    barrier();
    for( int i=-1; i<=2 ; i++){
    gridIndX =int(ParticleInGrid.x/gridSpacing) +i;

        for(int j=-1; j<=2; j++){
            gridIndY =int(ParticleInGrid.y/gridSpacing) +j;

            for(int k=-1 ;k<=2;k++ ){
                gridIndZ =int(ParticleInGrid.z/gridSpacing) +k;

                if(gridIndX>= n && gridIndY>=n && gridIndZ>=n  &&  gridIndX< gGridDim[0].x &&  gridIndY <gGridDim[1].x   &&  gridIndZ< gGridDim[2].x ){
                    gIndex = getIndex(gridIndX,gridIndY,gridIndZ);
                    gVelocities[gIndex].xyz+=pVelocities[index].xyz;
                }
            }
        }
    }
    //97FPS
}
