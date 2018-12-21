#include "p2g_shared_atomic.hpp"

void P2G_shared_atomic::init(P2GData&& data, IOBufferData&& io) {
  auto shader = std::make_shared<Shader>(ShaderType::COMPUTE, filename);
  shader->set_local_size(local_size);

  std::vector<Shader::CommandType> vec = {};
  auto io_cmds(io.generateCommands());
  vec.insert(std::end(vec), std::begin(io_cmds), std::end(io_cmds));

  shader->add_cmds(vec.begin(), vec.end());

  Technique::add_shader(std::move(shader));
  Technique::upload();
  Technique::use();
  uniforms_init(std::move(data.uniforms));
}
void P2G_shared_atomic::init_atomic(P2GData&& data, IOBufferData&& io) {
  filename = "shader/test/shared/p2g_atomic.glsl";
  init(std::move(data), std::move(io));
}
void P2G_shared_atomic::uniforms_init(UniformsStatic&& uniforms) {
  Technique::uniform_update("gGridPos", uniforms.gGridPos.x,
                            uniforms.gGridPos.y, uniforms.gGridPos.z);
  Technique::uniform_update("gGridDim", uniforms.gGridDim);
  Technique::uniform_update("gridSpacing", uniforms.gridSpacing);
  gGridDim = uniforms.gGridDim;
}
void P2G_shared_atomic::dispatch(UniformsDynamic&& uniforms) {
  Technique::use();
  uniforms_update(std::move(uniforms));
  glDispatchCompute(gGridDim.x / local_size.x, gGridDim.y / local_size.y,
                    gGridDim.z / local_size.z);
}
void P2G_shared_atomic::dispatch_with_barrier(UniformsDynamic&& uniforms) {
  dispatch(std::move(uniforms));
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}
void P2G_shared_atomic::uniforms_update(UniformsDynamic&& uniforms) {}

