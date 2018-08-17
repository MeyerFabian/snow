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
  /*
  cMass.setGridPos(grid->x_off, grid->y_off, grid->z_off);
  cMass.setGridDim(grid->dimx, grid->dimy, grid->dimz);
  cMass.setGridSpacing(grid->h);
  /*/
  cMass.uniform_update("gGridPos", grid->x_off, grid->y_off, grid->z_off);
  cMass.uniform_update("gGridDim", grid->dimx, grid->dimy, grid->dimz);
  cMass.uniform_update("gridSpacing", grid->h);
  //*/
  cMass.setIndexSize(particlesystem->particles->size());

  glDispatchCompute(
      (particlesystem->particles->size()) / NUM_OF_GPGPU_THREADS_X + 1,
      PARTICLE_TO_GRID_SIZE, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  cVolume.plugTechnique();
  /*
  cVolume.setGridPos(grid->x_off, grid->y_off, grid->z_off);
  cVolume.setGridDim(grid->dimx, grid->dimy, grid->dimz);
  cVolume.setGridSpacing(grid->h);
  /*/
  cVolume.uniform_update("gGridPos", grid->x_off, grid->y_off, grid->z_off);
  cVolume.uniform_update("gGridDim", grid->dimx, grid->dimy, grid->dimz);
  cVolume.uniform_update("gridSpacing", grid->h);
  //*/
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
  /*
  rigidSim.setDt(dt);
  /*/
  rigidSim.uniform_update("dt", dt);
  //*/
  glDispatchCompute(collisionObjects->colliders->size(), 1, 1);

  collisionObjects->updateRenderBuffer(dt);
  rg.plugTechnique();
  glDispatchCompute(
      GRID_DIM_X * GRID_DIM_Y * GRID_DIM_Z / NUM_OF_GPGPU_THREADS_X + 1, 1, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  p2g.plugTechnique();
  /*
  p2g.setGridPos(grid->x_off, grid->y_off, grid->z_off);
  p2g.setGridDim(grid->dimx, grid->dimy, grid->dimz);
  p2g.setGridSpacing(grid->h);
  p2g.setYoung();
  p2g.setPoisson();
  p2g.setHardening();
  /*/
  p2g.uniform_update("gGridPos", grid->x_off, grid->y_off, grid->z_off);
  p2g.uniform_update("gGridDim", grid->dimx, grid->dimy, grid->dimz);
  p2g.uniform_update("gridSpacing", grid->h);
  p2g.uniform_update("young", YOUNG_MODULUS);
  p2g.uniform_update("poisson", POISSON);
  p2g.uniform_update("hardening", HARDENING);
  //*/
  p2g.setIndexSize(particlesystem->particles->size());
  glDispatchCompute(
      (particlesystem->particles->size()) / NUM_OF_GPGPU_THREADS_X + 1,
      PARTICLE_TO_GRID_SIZE, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  g2g.plugTechnique();
  /*
  g2g.setDt(dt);
  /*/
  g2g.uniform_update("dt", dt);
  //*/
  g2g.setnumColliders(collisionObjects->colliders->size());
  glDispatchCompute(
      GRID_DIM_X * GRID_DIM_Y * GRID_DIM_Z / NUM_OF_GPGPU_THREADS_X + 1, 1, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  g2p.plugTechnique();
  /*
  g2p.setGridPos(grid->x_off, grid->y_off, grid->z_off);
  g2p.setGridDim(grid->dimx, grid->dimy, grid->dimz);
  g2p.setGridSpacing(grid->h);
  /*/
  g2p.uniform_update("gGridPos", grid->x_off, grid->y_off, grid->z_off);
  g2p.uniform_update("gGridDim", grid->dimx, grid->dimy, grid->dimz);
  g2p.uniform_update("gridSpacing", grid->h);
  //*/
  g2p.setIndexSize(particlesystem->particles->size());
  glDispatchCompute(
      (particlesystem->particles->size()) / NUM_OF_GPGPU_THREADS_X + 1,
      PARTICLE_TO_GRID_SIZE, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  pU.plugTechnique();
  /*
  pU.setDt(dt);
  pU.setCritComp();
  pU.setCritStretch();
  /*/
  pU.uniform_update("dt", dt);
  pU.uniform_update("critComp", CRIT_COMPRESSION);
  pU.uniform_update("critStretch", CRIT_STRETCH);
  //*/
  pU.setIndexSize(particlesystem->particles->size());
  pU.setnumColliders(collisionObjects->colliders->size());
  glDispatchCompute(
      (particlesystem->particles->size()) / NUM_OF_GPGPU_THREADS_X + 1, 1, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

