#include "p2gCompute.hpp"
void P2GCompute::init(UniformsInit&& uniforms) {
  use();
  Technique::add_shader(std::make_shared<Shader>(
      ShaderType::COMPUTE, "shader/updateGridMassVel.glsl"));
  Technique::upload();
  uniforms_init(std::move(uniforms));
}
void P2GCompute::uniforms_init(UniformsInit&& uniforms) {
  uniform_update("gGridPos", GRID_POS_X, GRID_POS_Y, GRID_POS_Z);
  uniform_update("gGridDim", GRID_DIM_X, GRID_DIM_Y, GRID_DIM_Z);
  uniform_update("gridSpacing", GRID_SPACING);
  uniform_update("young", YOUNG_MODULUS);
  uniform_update("poisson", POISSON);
  uniform_update("hardening", HARDENING);
  uniform_update("indexSize", uniforms.numParticles);
  numParticles = uniforms.numParticles;
}
void P2GCompute::dispatch(UniformsUpdate&& uniforms) {
  use();
  uniforms_update(std::move(uniforms));
  glDispatchCompute((numParticles) / NUM_OF_GPGPU_THREADS_X + 1,
                    PARTICLE_TO_GRID_SIZE, 1);

  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void P2GCompute::uniforms_update(UniformsUpdate&& uniforms) {}

