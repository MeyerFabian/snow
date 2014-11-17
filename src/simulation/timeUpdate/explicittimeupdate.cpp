#include "explicittimeupdate.h"

void ExplicitTimeUpdate::init(){
    const char* pPCFileName = "shader/computeShader.glsl";

    string cs;
    if(!ReadFile(pPCFileName,cs)){
      fprintf(stderr, "Error: vs\n");
       exit(1);
    };
    pc = ParticleCompute();
    particlesystem->initSSBO();
    grid->initSSBO();
    pc.init(cs);

}


void ExplicitTimeUpdate::update(double dt){

    pc.plugTechnique();
    pc.setGridPos(grid->x_off, grid->y_off, grid->z_off);
    pc.setGridDim(grid->dimx, grid->dimy, grid->dimz);
    pc.setdt(dt);
    pc.setGridSpacing(grid->h);
    glDispatchCompute(65535,1,1);
    particlesystem->updateSSBOBuffer();
    glMemoryBarrier ( GL_SHADER_IMAGE_ACCESS_BARRIER_BIT );

}
