#include "g2pTransfer.hpp"
void G2PTransfer::init(UniformsStatic&& uniforms) {
  Technique::add_shader(
      std::make_shared<Shader>(ShaderType::COMPUTE, "shader/compute/mpm/g2pTransfer.glsl"));
  Technique::upload();
  Technique::use();
  uniforms_init(std::move(uniforms));
}
void G2PTransfer::uniforms_init(UniformsStatic&& uniforms) {
  Technique::uniform_update("gGridPos", GRID_POS_X, GRID_POS_Y, GRID_POS_Z);
  Technique::uniform_update("gGridDim", GRID_DIM_X, GRID_DIM_Y, GRID_DIM_Z);
  Technique::uniform_update("gridSpacing", GRID_SPACING);
  Technique::uniform_update("indexSize", uniforms.numParticles);
  m_numParticles = uniforms.numParticles;
}
void G2PTransfer::dispatch(UniformsDynamic&& uniforms) {
  Technique::use();
  uniforms_update(std::move(uniforms));
  glDispatchCompute((m_numParticles) / NUM_OF_GPGPU_THREADS_X + 1,
                    PARTICLE_TO_GRID_SIZE, 1);
}
void G2PTransfer::dispatch_with_barrier(UniformsDynamic&& uniforms) {
  dispatch(std::move(uniforms));
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}
void G2PTransfer::uniforms_update(UniformsDynamic&& uniforms) {}

