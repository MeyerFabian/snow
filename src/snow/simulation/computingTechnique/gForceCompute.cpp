#include "gForceCompute.hpp"
void GForceCompute::init(UniformsStatic&& uniforms) {
  Technique::add_shader(std::make_shared<Shader>(ShaderType::COMPUTE,
                                                 "shader/gForceCompute.glsl"));
  Technique::upload();
  Technique::use();
  uniforms_init(std::move(uniforms));
}
void GForceCompute::uniforms_init(UniformsStatic&& uniforms) {
  Technique::uniform_update("gNumColliders", uniforms.numColliders);
}
void GForceCompute::dispatch(UniformsDynamic&& uniforms) {
  Technique::use();
  uniforms_update(std::move(uniforms));
  glDispatchCompute(
      GRID_DIM_X * GRID_DIM_Y * GRID_DIM_Z / NUM_OF_GPGPU_THREADS_X + 1, 1, 1);
}
void GForceCompute::dispatch_with_barrier(UniformsDynamic&& uniforms) {
  dispatch(std::move(uniforms));
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}
void GForceCompute::uniforms_update(UniformsDynamic&& uniforms) {
  Technique::uniform_update("dt", uniforms.dt);
}

