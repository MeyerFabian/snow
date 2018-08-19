#include "explicittimeupdate.hpp"
#include "../../utils/benchmarker.hpp"

void ExplicitTimeUpdate::init() {
  scene.particleSys->initSSBO();
  scene.grid->initSSBO();
  scene.colliderSys->initSSBO();

  rg.init("shader/resetGrid.glsl");

  rigidSim.init("shader/updateRigids.glsl");

  cMass.init("shader/computeMass.glsl");
  cMass.use();
  cMass.uniform_update("gGridPos", GRID_POS_X, GRID_POS_Y, GRID_POS_Z);
  cMass.uniform_update("gGridDim", GRID_DIM_X, GRID_DIM_Y, GRID_DIM_Z);
  cMass.uniform_update("gridSpacing", GRID_SPACING);
  cMass.uniform_update("indexSize", numParticles);

  cVolume.init("shader/computeParticleVolume.glsl");
  cVolume.use();
  cVolume.uniform_update("gGridPos", GRID_POS_X, GRID_POS_Y, GRID_POS_Z);
  cVolume.uniform_update("gGridDim", GRID_DIM_X, GRID_DIM_Y, GRID_DIM_Z);
  cVolume.uniform_update("gridSpacing", GRID_SPACING);
  cVolume.uniform_update("indexSize", numParticles);

  p2g.init("shader/updateGridMassVel.glsl");
  p2g.use();
  p2g.uniform_update("gGridPos", GRID_POS_X, GRID_POS_Y, GRID_POS_Z);
  p2g.uniform_update("gGridDim", GRID_DIM_X, GRID_DIM_Y, GRID_DIM_Z);
  p2g.uniform_update("gridSpacing", GRID_SPACING);
  p2g.uniform_update("young", YOUNG_MODULUS);
  p2g.uniform_update("poisson", POISSON);
  p2g.uniform_update("hardening", HARDENING);
  p2g.uniform_update("indexSize", numParticles);

  g2g.init("shader/updateGridVelCollision.glsl");
  g2g.uniform_update("gNumColliders", numColliders);

  g2p.init("shader/updateParticleVel.glsl");
  g2p.use();
  g2p.uniform_update("gGridPos", GRID_POS_X, GRID_POS_Y, GRID_POS_Z);
  g2p.uniform_update("gGridDim", GRID_DIM_X, GRID_DIM_Y, GRID_DIM_Z);
  g2p.uniform_update("gridSpacing", GRID_SPACING);
  g2p.uniform_update("indexSize", numParticles);

  pU.init("shader/updateParticles.glsl");
  pU.use();
  pU.uniform_update("critComp", CRIT_COMPRESSION);
  pU.uniform_update("critStretch", CRIT_STRETCH);
  pU.uniform_update("gNumColliders", numColliders);
  pU.uniform_update("indexSize", numParticles);
  rg.use();
  glDispatchCompute(
      GRID_DIM_X * GRID_DIM_Y * GRID_DIM_Z / NUM_OF_GPGPU_THREADS_X + 1, 1, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  cMass.use();
  glDispatchCompute((numParticles) / NUM_OF_GPGPU_THREADS_X + 1,
                    PARTICLE_TO_GRID_SIZE, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  cVolume.use();
  glDispatchCompute((numParticles) / NUM_OF_GPGPU_THREADS_X + 1,
                    PARTICLE_TO_GRID_SIZE, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void ExplicitTimeUpdate::update(double dt) {
  rigidSim.use();
  rigidSim.uniform_update("dt", dt);
  glDispatchCompute(numColliders, 1, 1);

  scene.colliderSys->updateRenderBuffer(dt);

  rg.use();
  glDispatchCompute(
      GRID_DIM_X * GRID_DIM_Y * GRID_DIM_Z / NUM_OF_GPGPU_THREADS_X + 1, 1, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  p2g.use();
  glDispatchCompute((numParticles) / NUM_OF_GPGPU_THREADS_X + 1,
                    PARTICLE_TO_GRID_SIZE, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  g2g.use();
  g2g.uniform_update("dt", dt);
  glDispatchCompute(
      GRID_DIM_X * GRID_DIM_Y * GRID_DIM_Z / NUM_OF_GPGPU_THREADS_X + 1, 1, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  g2p.use();
  glDispatchCompute((numParticles) / NUM_OF_GPGPU_THREADS_X + 1,
                    PARTICLE_TO_GRID_SIZE, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  // scene.particleSys->debug();
  // scene.grid->debug();
  pU.use();
  pU.uniform_update("dt", dt);
  glDispatchCompute((numParticles) / NUM_OF_GPGPU_THREADS_X + 1, 1, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

