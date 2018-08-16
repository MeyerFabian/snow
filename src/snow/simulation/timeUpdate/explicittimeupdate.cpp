#include "explicittimeupdate.hpp"

void ExplicitTimeUpdate::init() {
  particlesystem->initSSBO();
  grid->initSSBO();
  collisionObjects->initSSBO();

  rg.init("shader/resetGrid.glsl");

  rigidSim.init("shader/updateRigids.glsl");

  cMass.init("shader/computeMass.glsl");
  cMass.uniform_update("gGridPos", grid->x_off, grid->y_off, grid->z_off);
  cMass.uniform_update("gGridDim", grid->dimx, grid->dimy, grid->dimz);
  cMass.uniform_update("gridSpacing", grid->h);
  cMass.uniform_update("indexSize",
                       static_cast<int>(particlesystem->particles->size()));

  cVolume.init("shader/computeParticleVolume.glsl");
  cVolume.uniform_update("gGridPos", grid->x_off, grid->y_off, grid->z_off);
  cVolume.uniform_update("gGridDim", grid->dimx, grid->dimy, grid->dimz);
  cVolume.uniform_update("gridSpacing", grid->h);
  cVolume.uniform_update("indexSize",
                         static_cast<int>(particlesystem->particles->size()));

  p2g.init("shader/updateGridMassVel.glsl");
  p2g.uniform_update("gGridPos", grid->x_off, grid->y_off, grid->z_off);
  p2g.uniform_update("gGridDim", grid->dimx, grid->dimy, grid->dimz);
  p2g.uniform_update("gridSpacing", grid->h);
  p2g.uniform_update("young", YOUNG_MODULUS);
  p2g.uniform_update("poisson", POISSON);
  p2g.uniform_update("hardening", HARDENING);
  p2g.uniform_update("indexSize",
                     static_cast<int>(particlesystem->particles->size()));

  g2g.init("shader/updateGridVelCollision.glsl");
  g2g.uniform_update("gNumColliders",
                     static_cast<int>(collisionObjects->colliders->size()));

  g2p.init("shader/updateParticleVel.glsl");
  g2p.uniform_update("gGridPos", grid->x_off, grid->y_off, grid->z_off);
  g2p.uniform_update("gGridDim", grid->dimx, grid->dimy, grid->dimz);
  g2p.uniform_update("gridSpacing", grid->h);
  g2p.uniform_update("indexSize",
                     static_cast<int>(particlesystem->particles->size()));

  pU.init("shader/updateParticles.glsl");
  pU.uniform_update("critComp", CRIT_COMPRESSION);
  pU.uniform_update("critStretch", CRIT_STRETCH);
  pU.uniform_update("gNumColliders",
                    static_cast<int>(collisionObjects->colliders->size()));
  pU.uniform_update("indexSize",
                    static_cast<int>(particlesystem->particles->size()));
  rg.use();
  glDispatchCompute(
      GRID_DIM_X * GRID_DIM_Y * GRID_DIM_Z / NUM_OF_GPGPU_THREADS_X + 1, 1, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  particlesystem->debug();

  cMass.use();
  glDispatchCompute((static_cast<int>(particlesystem->particles->size())) /
                            NUM_OF_GPGPU_THREADS_X +
                        1,
                    PARTICLE_TO_GRID_SIZE, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  cVolume.use();
  glDispatchCompute((static_cast<int>(particlesystem->particles->size())) /
                            NUM_OF_GPGPU_THREADS_X +
                        1,
                    PARTICLE_TO_GRID_SIZE, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  particlesystem->debug();
}

void ExplicitTimeUpdate::update(double dt) {
  // std::cout<<"Frame begin:"<<std::endl;
  g2g.uniform_update("dt", dt);
  rigidSim.uniform_update("dt", dt);
  pU.uniform_update("dt", dt);

  collisionObjects->updateRenderBuffer(dt);
  rigidSim.use();
  glDispatchCompute(static_cast<int>(collisionObjects->colliders->size()), 1,
                    1);

  rg.use();
  glDispatchCompute(
      GRID_DIM_X * GRID_DIM_Y * GRID_DIM_Z / NUM_OF_GPGPU_THREADS_X + 1, 1, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  p2g.use();
  glDispatchCompute((static_cast<int>(particlesystem->particles->size())) /
                            NUM_OF_GPGPU_THREADS_X +
                        1,
                    PARTICLE_TO_GRID_SIZE, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  g2g.use();
  glDispatchCompute(
      GRID_DIM_X * GRID_DIM_Y * GRID_DIM_Z / NUM_OF_GPGPU_THREADS_X + 1, 1, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  g2p.use();
  glDispatchCompute((static_cast<int>(particlesystem->particles->size())) /
                            NUM_OF_GPGPU_THREADS_X +
                        1,
                    PARTICLE_TO_GRID_SIZE, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  // scene.particleSys->debug();
  // scene.grid->debug();
  pU.use();
  glDispatchCompute((static_cast<int>(particlesystem->particles->size())) /
                            NUM_OF_GPGPU_THREADS_X +
                        1,
                    1, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

