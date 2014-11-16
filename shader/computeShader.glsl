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
/*
layout(std140, binding = 1) buffer pVel {
    vec4 pVelocities[ ];
};

layout(std140, binding = 4) buffer gVel {
    vec4 gVelocities[ ];
};
*/

layout(local_size_x= 1024, local_size_y =1, local_size_z = 1)in;

int getIndex(int i ,  int j , int k){
    return i + (j * int(gGridDim[0].x)) + (k *int(gGridDim[1].x) * int(gGridDim[0].x));
}

void main(void){
    uint index = gl_GlobalInvocationID.x;
    vec3 ParticleInGrid = (pPositionsMass[index].xyz- gGridPos);
    float ParticleMass = pPositionsMass[index].w;
    //for(int i = 0; i< 500;i++)
    pPositionsMass[index].x +=0.0005;
/*
    gPositionsMass[3].x+=0.000005;
gPositionsMass[7].x+=0.000005;
gPositionsMass[11].x+=0.000005;
gPositionsMass[15].x+=0.000005;
gPositionsMass[19].x+=0.000005;
gPositionsMass[863].x+=0.000005;
*/

    for( int i=-1; i<=2 ; i++){
    int gridIndX =int(ParticleInGrid.x/gridSpacing) +i;

        for(int j=-1; j<=2; j++){
            int gridIndY =int(ParticleInGrid.y/gridSpacing) +j;

            for(int k=-1 ;k<=2;k++ ){
                int gridIndZ =int(ParticleInGrid.z/gridSpacing) +k;
                int n= 0;
                if(gridIndX>= n  &&  gridIndX< gGridDim[0].x && gridIndY>=n  &&  gridIndY <gGridDim[1].x && gridIndZ>=n  &&  gridIndZ< gGridDim[2].x )
                    gPositionsMass[getIndex(gridIndX,gridIndY,gridIndZ)].w+=ParticleMass;
            }
        }
    }


}
