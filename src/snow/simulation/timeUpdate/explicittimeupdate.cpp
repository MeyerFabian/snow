#include "explicittimeupdate.hpp"

void ExplicitTimeUpdate::init() {
  particlesystem->initSSBO();
  grid->initSSBO();
  collisionObjects->initSSBO();

  rg.init("shader/resetGrid.glsl");

  rigidSim.init("shader/updateRigids.glsl");

  cMass.init("shader/computeMass.glsl");

  cVolume.init("shader/computeParticleVolume.glsl");

  p2g.init("shader/updateGridMassVel.glsl");

  g2g.init("shader/updateGridVelCollision.glsl");

  g2p.init("shader/updateParticleVel.glsl");

  pU.init("shader/updateParticles.glsl");

  rg.plugTechnique();
  glDispatchCompute(
      GRID_DIM_X * GRID_DIM_Y * GRID_DIM_Z / NUM_OF_GPGPU_THREADS_X + 1, 1, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  cMass.plugTechnique();
  /*
  cMass.setGridPos(grid->x_off, grid->y_off, grid->z_off);
  cMass.setGridDim(grid->dimx, grid->dimy, grid->dimz);
  cMass.setGridSpacing(grid->h);
  cMass.setIndexSize(numParticles);
  /*/
  cMass.uniform_update("gGridPos", grid->x_off, grid->y_off, grid->z_off);
  cMass.uniform_update("gGridDim", grid->dimx, grid->dimy, grid->dimz);
  cMass.uniform_update("gridSpacing", grid->h);
  cMass.uniform_update("indexSize", numParticles);

  //*/

  glDispatchCompute(numParticles / NUM_OF_GPGPU_THREADS_X + 1,
                    PARTICLE_TO_GRID_SIZE, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  cVolume.plugTechnique();
  /*
  cVolume.setGridPos(grid->x_off, grid->y_off, grid->z_off);
  cVolume.setGridDim(grid->dimx, grid->dimy, grid->dimz);
  cVolume.setGridSpacing(grid->h);
  cVolume.setIndexSize(numParticles);
  /*/
  cVolume.uniform_update("gGridPos", grid->x_off, grid->y_off, grid->z_off);
  cVolume.uniform_update("gGridDim", grid->dimx, grid->dimy, grid->dimz);
  cVolume.uniform_update("indexSize", numParticles);
  cVolume.uniform_update("gridSpacing", grid->h);
  //*/

  glDispatchCompute(numParticles / NUM_OF_GPGPU_THREADS_X + 1,
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
  glDispatchCompute(numColliders, 1, 1);

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
  p2g.setIndexSize(numParticles);
  /*/
  p2g.uniform_update("gGridPos", grid->x_off, grid->y_off, grid->z_off);
  p2g.uniform_update("gGridDim", grid->dimx, grid->dimy, grid->dimz);
  p2g.uniform_update("gridSpacing", grid->h);
  p2g.uniform_update("young", YOUNG_MODULUS);
  p2g.uniform_update("poisson", POISSON);
  p2g.uniform_update("hardening", HARDENING);
  p2g.uniform_update("indexSize", numParticles);
  //*/
  //
  glDispatchCompute(numParticles / NUM_OF_GPGPU_THREADS_X + 1,
                    PARTICLE_TO_GRID_SIZE, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  g2g.plugTechnique();
  /*
  g2g.setDt(dt);
  g2g.setnumColliders(numParticles);
  /*/
  g2g.uniform_update("dt", dt);
  g2g.uniform_update("gNumColliders", numColliders);
  //*/
  glDispatchCompute(
      GRID_DIM_X * GRID_DIM_Y * GRID_DIM_Z / NUM_OF_GPGPU_THREADS_X + 1, 1, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  g2p.plugTechnique();
  /*
  g2p.setGridPos(grid->x_off, grid->y_off, grid->z_off);
  g2p.setGridDim(grid->dimx, grid->dimy, grid->dimz);
  g2p.setGridSpacing(grid->h);
  g2p.setIndexSize(numParticles);
  /*/
  g2p.uniform_update("gGridPos", grid->x_off, grid->y_off, grid->z_off);
  g2p.uniform_update("gGridDim", grid->dimx, grid->dimy, grid->dimz);
  g2p.uniform_update("gridSpacing", grid->h);
  g2p.uniform_update("indexSize", numParticles);
  //*/
  glDispatchCompute(numParticles / NUM_OF_GPGPU_THREADS_X + 1,
                    PARTICLE_TO_GRID_SIZE, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  pU.plugTechnique();
  /*
  pU.setDt(dt);
  pU.setCritComp();
  pU.setCritStretch();
pU.setIndexSize(numParticles);
  pU.setnumColliders(numColliders);
  /*/
  pU.uniform_update("dt", dt);
  pU.uniform_update("critComp", CRIT_COMPRESSION);
  pU.uniform_update("critStretch", CRIT_STRETCH);
  pU.uniform_update("indexSize", numParticles);
  pU.uniform_update("gNumColliders", numColliders);
  //*/

  glDispatchCompute(numParticles / NUM_OF_GPGPU_THREADS_X + 1, 1, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

