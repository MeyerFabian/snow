#include "rigidAdvance.hpp"

void RigidAdvance::init(UniformsStatic&& uniforms) {
  Technique::add_shader(std::make_shared<Shader>(ShaderType::COMPUTE,
                                                 "shader/compute/mpm/rigidAdvance.glsl"));
  Technique::upload();
  Technique::use();
  uniforms_init(std::move(uniforms));
}

void RigidAdvance::uniforms_init(UniformsStatic&& uniforms) {}

void RigidAdvance::dispatch(UniformsDynamic&& uniforms) {
  Technique::use();
  uniforms_update(std::move(uniforms));

  glDispatchCompute(uniforms.numColliders, 1, 1);
}

void RigidAdvance::dispatch_with_barrier(UniformsDynamic&& uniforms) {
  dispatch(std::move(uniforms));
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void RigidAdvance::uniforms_update(UniformsDynamic&& uniforms) {
  Technique::uniform_update("dt", uniforms.dt);
}

