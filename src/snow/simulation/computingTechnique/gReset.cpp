#include "gReset.hpp"
void GReset::init(UniformsInit&& uniforms) {
  Technique::add_shader(
      std::make_shared<Shader>(ShaderType::COMPUTE, "shader/compute/mpm/gReset.glsl"));
  Technique::upload();
  Technique::use();
  uniforms_init(std::move(uniforms));
}
void GReset::uniforms_init(UniformsInit&& uniforms) {}
void GReset::dispatch(UniformsUpdate&& uniforms) {
  Technique::use();
  uniforms_update(std::move(uniforms));
  glDispatchCompute(
      GRID_DIM_X * GRID_DIM_Y * GRID_DIM_Z / NUM_OF_GPGPU_THREADS_X + 1, 1, 1);

  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}
void GReset::dispatch_with_barrier(UniformsUpdate&& uniforms) {
  dispatch(std::move(uniforms));
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}
void GReset::uniforms_update(UniformsUpdate&& uniforms) {}

