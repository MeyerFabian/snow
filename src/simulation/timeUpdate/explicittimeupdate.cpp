#include "explicittimeupdate.h"

void ExplicitTimeUpdate::init(){
    const char* pPCFileName = "shader/updateGridMassVel.glsl";
    const char* pResetGridFileName ="shader/resetGrid.glsl";
    const char* pDivideByMass = "shader/overGridDivideVelByMass.glsl";
    string cs;

    if(!ReadFile(pPCFileName,cs)){
      fprintf(stderr, "Error: vs\n");
       exit(1);
    };
    pc = ParticleCompute();
    particlesystem->initSSBO();
    grid->initSSBO();
    pc.init(cs);

    cs.clear();
    if(!ReadFile(pResetGridFileName,cs)){
      fprintf(stderr, "Error: vs\n");
       exit(1);
    };
    rg = OverGrid();
    rg.init(cs);

    cs.clear();
    if(!ReadFile(pDivideByMass,cs)){
      fprintf(stderr, "Error: vs\n");
       exit(1);
    };
    divVelMass = OverGrid();
    divVelMass.init(cs);
}


void ExplicitTimeUpdate::update(double dt){

    rg.plugTechnique();
    glDispatchComputeGroupSizeARB(GRID_DIM_X * GRID_DIM_Y * GRID_DIM_Z/NUM_OF_GPGPU_THREADS_X,1,1,NUM_OF_GPGPU_THREADS_X,1,1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    pc.plugTechnique();
    pc.setGridPos(grid->x_off, grid->y_off, grid->z_off);
    pc.setGridDim(grid->dimx, grid->dimy, grid->dimz);
    pc.setdt(dt);
    pc.setGridSpacing(grid->h);
    glDispatchComputeGroupSizeARB(NUMOFPARTICLES/NUM_OF_GPGPU_THREADS_X,PARTICLE_TO_GRID_SIZE,1,NUM_OF_GPGPU_THREADS_X,1,1);
    glMemoryBarrier ( GL_SHADER_IMAGE_ACCESS_BARRIER_BIT );

    divVelMass.plugTechnique();
    glDispatchComputeGroupSizeARB(GRID_DIM_X * GRID_DIM_Y * GRID_DIM_Z/NUM_OF_GPGPU_THREADS_X,1,1,NUM_OF_GPGPU_THREADS_X,1,1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);


}
