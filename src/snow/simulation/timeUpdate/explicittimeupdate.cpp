#include "explicittimeupdate.hpp"
#include "../../utils/benchmarker.hpp"

void ExplicitTimeUpdate::init() {
  scene.particleSys->initSSBO();
  scene.grid->initSSBO();
  scene.colliderSys->initSSBO();

  // Initial all shader programs, shaders and uniforms
  scratch_grid.init({});

  advance_rigids.init({});

  compute_grid_mass.init({numParticles});

  compute_particle_volumes.init({numParticles});

  transfer_to_grid.init({numParticles});

  compute_grid_derivates.init({numColliders});

  transfer_back_to_particles.init({numParticles});

  advance_particles.init({numParticles, numColliders});

  // Compute the initial volume particles occupy and keep throughout the
  // simulation

  scratch_grid.dispatch_with_barrier({});

  compute_grid_mass.dispatch_with_barrier();

  compute_particle_volumes.dispatch_with_barrier();
}

void ExplicitTimeUpdate::update(double dt) {
  // MPM-Explicit update procedure

  BenchmarkerGPU::getInstance().time("#Sim:Advance Rigids", [this, &dt]() {
    return advance_rigids.dispatch({numColliders, dt});
  });

  scene.colliderSys->updateRenderBuffer(dt);

  BenchmarkerGPU::getInstance().time("#Sim:Scratch Grid", [this]() {
    return scratch_grid.dispatch_with_barrier({});
  });

  BenchmarkerGPU::getInstance().time("#Sim:Transfer to Grid", [this]() {
    return transfer_to_grid.dispatch_with_barrier({});
  });

  BenchmarkerGPU::getInstance().time("#Sim:Compute Derivatives", [this, &dt]() {
    return compute_grid_derivates.dispatch_with_barrier({dt});
  });

  BenchmarkerGPU::getInstance().time("#Sim:Transfer to Particles", [this]() {
    return transfer_back_to_particles.dispatch_with_barrier({});
  });
  BenchmarkerGPU::getInstance().time("#Sim:Advance Particles", [this, &dt]() {
    return advance_particles.dispatch_with_barrier({dt});
  });
}

