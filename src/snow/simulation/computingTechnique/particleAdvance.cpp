#include "particleAdvance.hpp"
void ParticleAdvance::init(UniformsStatic&& uniforms) {
  Technique::add_shader(std::make_shared<Shader>(
      ShaderType::COMPUTE, "shader/particleAdvance.glsl"));
  Technique::upload();
  Technique::use();
  uniforms_init(std::move(uniforms));
}

void ParticleAdvance::uniforms_init(UniformsStatic&& uniforms) {
  Technique::uniform_update("critComp", CRIT_COMPRESSION);
  Technique::uniform_update("critStretch", CRIT_STRETCH);
  Technique::uniform_update("gNumColliders", uniforms.numColliders);
  Technique::uniform_update("indexSize", uniforms.numParticles);
  m_numParticles = uniforms.numParticles;
}

void ParticleAdvance::dispatch(UniformsDynamic&& uniforms) {
  Technique::use();
  uniforms_update(std::move(uniforms));
  glDispatchCompute(m_numParticles / NUM_OF_GPGPU_THREADS_X + 1, 1, 1);
}

void ParticleAdvance::dispatch_with_barrier(UniformsDynamic&& uniforms) {
  dispatch(std::move(uniforms));
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void ParticleAdvance::uniforms_update(UniformsDynamic&& uniforms) {
  Technique::uniform_update("dt", uniforms.dt);
}

