#include "explicittimeupdate.hpp"

void ExplicitTimeUpdate::init() {
  particlesystem->initSSBO();
  grid->initSSBO();
  collisionObjects->initSSBO();

  rg.init("shader/resetGrid.glsl");

  rigidSim.init("shader/updateRigids.glsl");

  cMass.init("shader/computeMass.glsl");
  cMass.updateUniform("gGridPos", grid->x_off, grid->y_off, grid->z_off);
  cMass.updateUniform("gGridDim", grid->dimx, grid->dimy, grid->dimz);
  cMass.updateUniform("gridSpacing", grid->h);
  cMass.updateUniform("indexSize", particlesystem->particles->size());

  cVolume.init("shader/computeParticleVolume.glsl");
  cVolume.updateUniform("gGridPos", grid->x_off, grid->y_off, grid->z_off);
  cVolume.updateUniform("gGridDim", grid->dimx, grid->dimy, grid->dimz);
  cVolume.updateUniform("gridSpacing", grid->h);
  cVolume.updateUniform("indexSize", particlesystem->particles->size());

  p2g.init("shader/updateGridMassVel.glsl");
  p2g.updateUniform("gGridPos", grid->x_off, grid->y_off, grid->z_off);
  p2g.updateUniform("gGridDim", grid->dimx, grid->dimy, grid->dimz);
  p2g.updateUniform("gridSpacing", grid->h);
  p2g.updateUniform("young", YOUNG_MODULUS);
  p2g.updateUniform("poisson", POISSON);
  p2g.updateUniform("hardening", HARDENING);
  p2g.updateUniform("indexSize", particlesystem->particles->size());

  g2g.init("shader/updateGridVelCollision.glsl");
  g2g.updateUniform("gNumColliders", collisionObjects->colliders->size());

  g2p.init("shader/updateParticleVel.glsl");
  g2p.updateUniform("gGridPos", grid->x_off, grid->y_off, grid->z_off);
  g2p.updateUniform("gGridDim", grid->dimx, grid->dimy, grid->dimz);
  g2p.updateUniform("gridSpacing", grid->h);
  g2p.updateUniform("indexSize", particlesystem->particles->size());

  pU.init("shader/updateParticles.glsl");
  pU.updateUniform("critComp", CRIT_COMPRESSION);
  pU.updateUniform("critStretch", CRIT_STRETCH);
  pU.updateUniform("gNumColliders", collisionObjects->colliders->size());
  pU.updateUniform("indexSize", particlesystem->particles->size());
  rg.use();
  glDispatchCompute(
      GRID_DIM_X * GRID_DIM_Y * GRID_DIM_Z / NUM_OF_GPGPU_THREADS_X + 1, 1, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  particlesystem->debug();

  cMass.use();
  glDispatchCompute(
      (particlesystem->particles->size()) / NUM_OF_GPGPU_THREADS_X + 1,
      PARTICLE_TO_GRID_SIZE, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  cVolume.use();
  glDispatchCompute(
      (particlesystem->particles->size()) / NUM_OF_GPGPU_THREADS_X + 1,
      PARTICLE_TO_GRID_SIZE, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  particlesystem->debug();
}

void ExplicitTimeUpdate::update(double dt) {
  rigidSim.use();
  rigidSim.updateUniform("dt", dt);
  glDispatchCompute(collisionObjects->colliders->size(), 1, 1);

  collisionObjects->updateRenderBuffer(dt);

  rg.use();
  glDispatchCompute(
      GRID_DIM_X * GRID_DIM_Y * GRID_DIM_Z / NUM_OF_GPGPU_THREADS_X + 1, 1, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  p2g.use();
  glDispatchCompute(
      (particlesystem->particles->size()) / NUM_OF_GPGPU_THREADS_X + 1,
      PARTICLE_TO_GRID_SIZE, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  g2g.use();
  g2g.updateUniform("dt", dt);
  glDispatchCompute(
      GRID_DIM_X * GRID_DIM_Y * GRID_DIM_Z / NUM_OF_GPGPU_THREADS_X + 1, 1, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  g2p.use();
  glDispatchCompute(
      (particlesystem->particles->size()) / NUM_OF_GPGPU_THREADS_X + 1,
      PARTICLE_TO_GRID_SIZE, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  // particlesystem->debug();
  // grid->debug();
  pU.use();
  pU.updateUniform("dt", dt);
  glDispatchCompute(
      (particlesystem->particles->size()) / NUM_OF_GPGPU_THREADS_X + 1, 1, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

