#include "explicittimeupdate.hpp"

void ExplicitTimeUpdate::init() {
  particlesystem->initSSBO();
  grid->initSSBO();
  collisionObjects->initSSBO();

  rg.init("shader/resetGrid.glsl");
  cMass.init("shader/computeMass.glsl");
  cVolume.init("shader/computeParticleVolume.glsl");
  rigidSim.init("shader/updateRigids.glsl");
  p2g.init("shader/updateGridMassVel.glsl");
  g2g.init("shader/updateGridVelCollision.glsl");
  g2p.init("shader/updateParticleVel.glsl");
  pU.init("shader/updateParticles.glsl");

  rg.use();
  glDispatchCompute(
      GRID_DIM_X * GRID_DIM_Y * GRID_DIM_Z / NUM_OF_GPGPU_THREADS_X + 1, 1, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  particlesystem->debug();

  cMass.use();
  cMass.setGridPos(grid->x_off, grid->y_off, grid->z_off);
  cMass.setGridDim(grid->dimx, grid->dimy, grid->dimz);
  cMass.setGridSpacing(grid->h);
  cMass.setIndexSize(particlesystem->particles->size());
  glDispatchCompute(
      (particlesystem->particles->size()) / NUM_OF_GPGPU_THREADS_X + 1,
      PARTICLE_TO_GRID_SIZE, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  cVolume.use();
  cVolume.setGridPos(grid->x_off, grid->y_off, grid->z_off);
  cVolume.setGridDim(grid->dimx, grid->dimy, grid->dimz);
  cVolume.setGridSpacing(grid->h);

  cVolume.setIndexSize(particlesystem->particles->size());
  glDispatchCompute(
      (particlesystem->particles->size()) / NUM_OF_GPGPU_THREADS_X + 1,
      PARTICLE_TO_GRID_SIZE, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
  particlesystem->debug();
  // grid->debug();
}

void ExplicitTimeUpdate::update(double dt) {
  rigidSim.use();
  rigidSim.setDt(dt);
  glDispatchCompute(collisionObjects->colliders->size(), 1, 1);

  collisionObjects->updateRenderBuffer(dt);
  rg.use();
  glDispatchCompute(
      GRID_DIM_X * GRID_DIM_Y * GRID_DIM_Z / NUM_OF_GPGPU_THREADS_X + 1, 1, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  p2g.use();
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

  g2g.use();
  g2g.setDt(dt);
  g2g.setGridDim(grid->dimx, grid->dimy, grid->dimz);
  g2g.setCollisionOffset();
  g2g.setnumColliders(collisionObjects->colliders->size());
  glDispatchCompute(
      GRID_DIM_X * GRID_DIM_Y * GRID_DIM_Z / NUM_OF_GPGPU_THREADS_X + 1, 1, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  g2p.use();
  g2p.setGridPos(grid->x_off, grid->y_off, grid->z_off);
  g2p.setGridDim(grid->dimx, grid->dimy, grid->dimz);
  g2p.setGridSpacing(grid->h);

  g2p.setIndexSize(particlesystem->particles->size());
  glDispatchCompute(
      (particlesystem->particles->size()) / NUM_OF_GPGPU_THREADS_X + 1,
      PARTICLE_TO_GRID_SIZE, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  // particlesystem->debug();
  // grid->debug();
  pU.use();
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
}

