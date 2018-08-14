#include "explicittimeupdate.hpp"

void ExplicitTimeUpdate::init() {
  scene.particleSys->initSSBO();
  scene.grid->initSSBO();
  scene.colliderSys->initSSBO();

  rg.init("shader/resetGrid.glsl");

  rigidSim.init("shader/updateRigids.glsl");

  cMass.init("shader/computeMass.glsl");
  cMass.uniform_update("gGridPos", scene.grid->x_off, scene.grid->y_off,
                       scene.grid->z_off);
  cMass.uniform_update("gGridDim", scene.grid->dimx, scene.grid->dimy,
                       scene.grid->dimz);
  cMass.uniform_update("scene.gridSpacing", scene.grid->h);
  cMass.uniform_update("indexSize", scene.particleSys->particles.size());

  cVolume.init("shader/computeParticleVolume.glsl");
  cVolume.uniform_update("gGridPos", scene.grid->x_off, scene.grid->y_off,
                         scene.grid->z_off);
  cVolume.uniform_update("gGridDim", scene.grid->dimx, scene.grid->dimy,
                         scene.grid->dimz);
  cVolume.uniform_update("scene.gridSpacing", scene.grid->h);
  cVolume.uniform_update("indexSize", scene.particleSys->particles.size());

  p2g.init("shader/updateGridMassVel.glsl");
  p2g.uniform_update("gGridPos", scene.grid->x_off, scene.grid->y_off,
                     scene.grid->z_off);
  p2g.uniform_update("gGridDim", scene.grid->dimx, scene.grid->dimy,
                     scene.grid->dimz);
  p2g.uniform_update("scene.gridSpacing", scene.grid->h);
  p2g.uniform_update("young", YOUNG_MODULUS);
  p2g.uniform_update("poisson", POISSON);
  p2g.uniform_update("hardening", HARDENING);
  p2g.uniform_update("indexSize", scene.particleSys->particles.size());

  g2g.init("shader/updateGridVelCollision.glsl");
  g2g.uniform_update("gNumColliders", scene.colliderSys->colliders.size());

  g2p.init("shader/updateParticleVel.glsl");
  g2p.uniform_update("gGridPos", scene.grid->x_off, scene.grid->y_off,
                     scene.grid->z_off);
  g2p.uniform_update("gGridDim", scene.grid->dimx, scene.grid->dimy,
                     scene.grid->dimz);
  g2p.uniform_update("scene.gridSpacing", scene.grid->h);
  g2p.uniform_update("indexSize", scene.particleSys->particles.size());

  pU.init("shader/updateParticles.glsl");
  pU.uniform_update("critComp", CRIT_COMPRESSION);
  pU.uniform_update("critStretch", CRIT_STRETCH);
  pU.uniform_update("gNumColliders", scene.colliderSys->colliders.size());
  pU.uniform_update("indexSize", scene.particleSys->particles.size());
  rg.use();
  glDispatchCompute(
      GRID_DIM_X * GRID_DIM_Y * GRID_DIM_Z / NUM_OF_GPGPU_THREADS_X + 1, 1, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  scene.particleSys->debug();

  cMass.use();
  glDispatchCompute(
      (scene.particleSys->particles.size()) / NUM_OF_GPGPU_THREADS_X + 1,
      PARTICLE_TO_GRID_SIZE, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  cVolume.use();
  glDispatchCompute(
      (scene.particleSys->particles.size()) / NUM_OF_GPGPU_THREADS_X + 1,
      PARTICLE_TO_GRID_SIZE, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  scene.particleSys->debug();
}

void ExplicitTimeUpdate::update(double dt) {
  rigidSim.use();
  rigidSim.uniform_update("dt", dt);
  glDispatchCompute(scene.colliderSys->colliders.size(), 1, 1);

  scene.colliderSys->updateRenderBuffer(dt);

  rg.use();
  glDispatchCompute(
      GRID_DIM_X * GRID_DIM_Y * GRID_DIM_Z / NUM_OF_GPGPU_THREADS_X + 1, 1, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  p2g.use();
  glDispatchCompute(
      (scene.particleSys->particles.size()) / NUM_OF_GPGPU_THREADS_X + 1,
      PARTICLE_TO_GRID_SIZE, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  g2g.use();
  g2g.uniform_update("dt", dt);
  glDispatchCompute(
      GRID_DIM_X * GRID_DIM_Y * GRID_DIM_Z / NUM_OF_GPGPU_THREADS_X + 1, 1, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  g2p.use();
  glDispatchCompute(
      (scene.particleSys->particles.size()) / NUM_OF_GPGPU_THREADS_X + 1,
      PARTICLE_TO_GRID_SIZE, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  // scene.particleSys->debug();
  // scene.grid->debug();
  pU.use();
  pU.uniform_update("dt", dt);
  glDispatchCompute(
      (scene.particleSys->particles.size()) / NUM_OF_GPGPU_THREADS_X + 1, 1, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

