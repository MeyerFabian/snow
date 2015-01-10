#include "explicittimeupdate.h"

void ExplicitTimeUpdate::init(){
    const char* pComputeVolumeFileName = "shader/computeParticleVolume.glsl";
    const char* pMassFileName = "shader/computeMass.glsl";
    const char* pPCFileName = "shader/updateGridMassVel.glsl";
    const char* pResetGridFileName ="shader/resetGrid.glsl";
    const char* pUpdateGridVelocity = "shader/updateGridVelCollision.glsl";
    const char* pUpdateParticleVelocity = "shader/updateParticleVel.glsl";
    const char* pUpdateParticles = "shader/updateParticles.glsl";

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
    if(!ReadFile(pUpdateGridVelocity,cs)){
      fprintf(stderr, "Error: vs\n");
       exit(1);
    };
    vUp = OverGrid();
    vUp.init(cs);

    cs.clear();
    if(!ReadFile(pUpdateParticleVelocity,cs)){
      fprintf(stderr, "Error: vs\n");
       exit(1);
    };
    pVU = ParticleCompute();
    pVU.init(cs);

    cs.clear();
    if(!ReadFile(pUpdateParticles,cs)){
      fprintf(stderr, "Error: vs\n");
       exit(1);
    };
    pU = OverGrid();
    pU.init(cs);

    cs.clear();
    if(!ReadFile(pMassFileName,cs)){
      fprintf(stderr, "Error: vs\n");
       exit(1);
    };
    cMass = ParticleCompute();
    cMass.init(cs);

    cs.clear();
    if(!ReadFile(pComputeVolumeFileName,cs)){
      fprintf(stderr, "Error: vs\n");
       exit(1);
    };
    cVolume = ParticleCompute();
    cVolume.init(cs);
/*
    cs.clear();
    if(!ReadFile(pDivideByMass,cs)){
      fprintf(stderr, "Error: vs\n");
       exit(1);
    };
    divVelMass = OverGrid();
    divVelMass.init(cs);
*/
    rg.plugTechnique();
    glDispatchComputeGroupSizeARB(GRID_DIM_X * GRID_DIM_Y * GRID_DIM_Z/NUM_OF_GPGPU_THREADS_X,1,1,NUM_OF_GPGPU_THREADS_X,1,1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    cMass.plugTechnique();
    cMass.setGridPos(grid->x_off, grid->y_off, grid->z_off);
    cMass.setGridDim(grid->dimx, grid->dimy, grid->dimz);
    cMass.setGridSpacing(grid->h);
    glDispatchComputeGroupSizeARB(NUMOFPARTICLES/NUM_OF_GPGPU_THREADS_X,PARTICLE_TO_GRID_SIZE,1,NUM_OF_GPGPU_THREADS_X,1,1);
    glMemoryBarrier ( GL_SHADER_STORAGE_BARRIER_BIT );
    //grid->debug();
    cVolume.plugTechnique();
    cVolume.setGridPos(grid->x_off, grid->y_off, grid->z_off);
    cVolume.setGridDim(grid->dimx, grid->dimy, grid->dimz);
    cVolume.setGridSpacing(grid->h);
    glDispatchComputeGroupSizeARB(NUMOFPARTICLES/NUM_OF_GPGPU_THREADS_X,PARTICLE_TO_GRID_SIZE,1,NUM_OF_GPGPU_THREADS_X,1,1);
    glMemoryBarrier ( GL_SHADER_STORAGE_BARRIER_BIT );
    //particlesystem->debug();


}


void ExplicitTimeUpdate::update(double dt){
//std::cout<<"Frame begin:"<<std::endl;

    rg.plugTechnique();
    glDispatchComputeGroupSizeARB(GRID_DIM_X * GRID_DIM_Y * GRID_DIM_Z/NUM_OF_GPGPU_THREADS_X,1,1,NUM_OF_GPGPU_THREADS_X,1,1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    pc.plugTechnique();
    pc.setGridPos(grid->x_off, grid->y_off, grid->z_off);
    pc.setGridDim(grid->dimx, grid->dimy, grid->dimz);
    pc.setGridSpacing(grid->h);
    pc.setYoung();
    pc.setPoisson();
    pc.setHardening();
    pc.setCritComp();
    pc.setCritStretch();
    glDispatchComputeGroupSizeARB(NUMOFPARTICLES/NUM_OF_GPGPU_THREADS_X,PARTICLE_TO_GRID_SIZE,1,NUM_OF_GPGPU_THREADS_X,1,1);
    glMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT );


   // particlesystem->debug();

    vUp.plugTechnique();
    vUp.setDt(dt);
    vUp.setGridDim(grid->dimx, grid->dimy, grid->dimz);
    vUp.setCollisionOffset();
    glDispatchComputeGroupSizeARB(GRID_DIM_X * GRID_DIM_Y * GRID_DIM_Z/NUM_OF_GPGPU_THREADS_X,1,1,NUM_OF_GPGPU_THREADS_X,1,1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    //


    pVU.plugTechnique();
    pVU.setGridPos(grid->x_off, grid->y_off, grid->z_off);
    pVU.setGridDim(grid->dimx, grid->dimy, grid->dimz);
    pVU.setGridSpacing(grid->h);
    glDispatchComputeGroupSizeARB(NUMOFPARTICLES/NUM_OF_GPGPU_THREADS_X,PARTICLE_TO_GRID_SIZE,1,NUM_OF_GPGPU_THREADS_X,1,1);
    glMemoryBarrier ( GL_SHADER_STORAGE_BARRIER_BIT );


    //std::cout<<"Vor FEp Update"<<std::endl;

    //particlesystem->debug();
    //grid->debug();
    pU.plugTechnique();
    pU.setDt(dt);   
    pU.setCritComp();
    pU.setCritStretch();
    pU.setGridDim(grid->dimx, grid->dimy, grid->dimz);
    pU.setCollisionOffset();
    glDispatchComputeGroupSizeARB(NUMOFPARTICLES/NUM_OF_GPGPU_THREADS_X,1,1,NUM_OF_GPGPU_THREADS_X,1,1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    //std::cout<<"Nach FEp Update"<<std::endl;
   // grid->debug();


/*
 * obsolete can be done in previous compute shader
    divVelMass.plugTechnique();
    glDispatchComputeGroupSizeARB(GRID_DIM_X * GRID_DIM_Y * GRID_DIM_Z/NUM_OF_GPGPU_THREADS_X,1,1,NUM_OF_GPGPU_THREADS_X,1,1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
*/


}
