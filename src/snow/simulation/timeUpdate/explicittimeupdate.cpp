#include "explicittimeupdate.hpp"

void ExplicitTimeUpdate::init() {
  particlesystem->initSSBO();
  grid->initSSBO();
  collisionObjects->initSSBO();

  string cs;

  const char* pResetGridFileName = "shader/resetGrid.glsl";
  if (!ReadFile(pResetGridFileName, cs)) {
    fprintf(stderr, "Error: vs\n");
    exit(1);
  };
  rg.init(cs);

  cs.clear();
  const char* pMassFileName = "shader/computeMass.glsl";
  if (!ReadFile(pMassFileName, cs)) {
    fprintf(stderr, "Error: vs\n");
    exit(1);
  };
  cMass.init(cs);

  cs.clear();
  const char* pComputeVolumeFileName = "shader/computeParticleVolume.glsl";
  if (!ReadFile(pComputeVolumeFileName, cs)) {
    fprintf(stderr, "Error: vs\n");
    exit(1);
  };
  cVolume.init(cs);

  cs.clear();
  const char* pRigidFileName = "shader/updateRigids.glsl";
  if (!ReadFile(pRigidFileName, cs)) {
    fprintf(stderr, "Error: vs\n");
    exit(1);
  };
  rigidSim.init(cs);

  cs.clear();
  const char* pPCFileName = "shader/updateGridMassVel.glsl";
  if (!ReadFile(pPCFileName, cs)) {
    fprintf(stderr, "Error: vs\n");
    exit(1);
  };
  p2g.init(cs);

  cs.clear();
  const char* pUpdateGridVelocity = "shader/updateGridVelCollision.glsl";
  if (!ReadFile(pUpdateGridVelocity, cs)) {
    fprintf(stderr, "Error: vs\n");
    exit(1);
  };
  g2g.init(cs);

  cs.clear();
  const char* pUpdateParticleVelocity = "shader/updateParticleVel.glsl";
  if (!ReadFile(pUpdateParticleVelocity, cs)) {
    fprintf(stderr, "Error: vs\n");
    exit(1);
  };
  g2p.init(cs);

  cs.clear();
  const char* pUpdateParticles = "shader/updateParticles.glsl";
  if (!ReadFile(pUpdateParticles, cs)) {
    fprintf(stderr, "Error: vs\n");
    exit(1);
  };
  pU.init(cs);

  rg.plugTechnique();
  glDispatchCompute(
      GRID_DIM_X * GRID_DIM_Y * GRID_DIM_Z / NUM_OF_GPGPU_THREADS_X + 1, 1, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  cMass.plugTechnique();
  cMass.setGridPos(grid->x_off, grid->y_off, grid->z_off);
  cMass.setGridDim(grid->dimx, grid->dimy, grid->dimz);
  cMass.setGridSpacing(grid->h);
  cMass.setIndexSize(particlesystem->particles->size());
  glDispatchCompute(
      (particlesystem->particles->size()) / NUM_OF_GPGPU_THREADS_X + 1,
      PARTICLE_TO_GRID_SIZE, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  cVolume.plugTechnique();
  cVolume.setGridPos(grid->x_off, grid->y_off, grid->z_off);
  cVolume.setGridDim(grid->dimx, grid->dimy, grid->dimz);
  cVolume.setGridSpacing(grid->h);

  cVolume.setIndexSize(particlesystem->particles->size());
  glDispatchCompute(
      (particlesystem->particles->size()) / NUM_OF_GPGPU_THREADS_X + 1,
      PARTICLE_TO_GRID_SIZE, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
  // particlesystem->debug();
  // grid->debug();
}

void ExplicitTimeUpdate::update(double dt) {
  // std::cout<<"Frame begin:"<<std::endl;

  rigidSim.plugTechnique();
  rigidSim.setDt(dt);
  glDispatchCompute(collisionObjects->colliders->size(), 1, 1);

  collisionObjects->updateRenderBuffer(dt);
  rg.plugTechnique();
  glDispatchCompute(
      GRID_DIM_X * GRID_DIM_Y * GRID_DIM_Z / NUM_OF_GPGPU_THREADS_X + 1, 1, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  p2g.plugTechnique();
  p2g.setGridPos(grid->x_off, grid->y_off, grid->z_off);
  p2g.setGridDim(grid->dimx, grid->dimy, grid->dimz);
  p2g.setGridSpacing(grid->h);
  p2g.setYoung();
  p2g.setPoisson();
  p2g.setHardening();
  p2g.setCritComp();
  p2g.setCritStretch();

  p2g.setIndexSize(particlesystem->particles->size());
  glDispatchCompute(
      (particlesystem->particles->size()) / NUM_OF_GPGPU_THREADS_X + 1,
      PARTICLE_TO_GRID_SIZE, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  g2g.plugTechnique();
  g2g.setDt(dt);
  g2g.setGridDim(grid->dimx, grid->dimy, grid->dimz);
  g2g.setCollisionOffset();
  g2g.setnumColliders(collisionObjects->colliders->size());
  glDispatchCompute(
      GRID_DIM_X * GRID_DIM_Y * GRID_DIM_Z / NUM_OF_GPGPU_THREADS_X + 1, 1, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
  //
  // grid->debug();

  g2p.plugTechnique();
  g2p.setGridPos(grid->x_off, grid->y_off, grid->z_off);
  g2p.setGridDim(grid->dimx, grid->dimy, grid->dimz);
  g2p.setGridSpacing(grid->h);

  g2p.setIndexSize(particlesystem->particles->size());
  glDispatchCompute(
      (particlesystem->particles->size()) / NUM_OF_GPGPU_THREADS_X + 1,
      PARTICLE_TO_GRID_SIZE, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  // particlesystem->debug();
  // particlesystem->debug();
  // grid->debug();
  // std::cout<<"Vor FEp Update"<<std::endl;

  // particlesystem->debug();
  // grid->debug();
  pU.plugTechnique();
  pU.setDt(dt);
  pU.setCritComp();
  pU.setCritStretch();
  pU.setGridDim(grid->dimx, grid->dimy, grid->dimz);
  pU.setCollisionOffset();
  pU.setIndexSize(particlesystem->particles->size());
  pU.setnumColliders(collisionObjects->colliders->size());
  glDispatchCompute(
      (particlesystem->particles->size()) / NUM_OF_GPGPU_THREADS_X + 1, 1, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  // std::cout<<"Nach FEp Update"<<std::endl;

  /*
   * obsolete can be done in previous compute shader
      divVelMass.plugTechnique();
      glDispatchCompute(GRID_DIM_X * GRID_DIM_Y *
   GRID_DIM_Z/NUM_OF_GPGPU_THREADS_X,1,1,NUM_OF_GPGPU_THREADS_X,1,1);
      glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
  */
}

