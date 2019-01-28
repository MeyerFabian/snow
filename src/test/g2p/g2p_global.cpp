#include "g2p_global.hpp"

void G2P_global::init(G2PData&& data, IOBufferData&& io) {
  auto shader = std::make_shared<Shader>(ShaderType::COMPUTE, filename);
  shader->set_local_size(local_size);

  std::vector<Shader::CommandType> vec = {};

  auto io_cmds(io.generateCommands());
  vec.insert(std::end(vec), std::begin(io_cmds), std::end(io_cmds));

  if (data.apic) {
    vec.insert(std::end(vec), {PreprocessorCmd::DEFINE, "APIC"});
  }

  shader->add_cmds(vec.begin(), vec.end());

  Technique::add_shader(std::move(shader));
  Technique::upload();
  Technique::use();
}
void G2P_global::init_looping(G2PData&& data, IOBufferData&& io) {
  filename = "shader/test/global/g2p.glsl";
  init(std::move(data), std::move(io));
}
void G2P_global::dispatch(UniformsDynamic&& uniforms) {
  Technique::use();
  auto numParticles = uniforms.numParticles;
  uniforms_update(std::move(uniforms));
  glDispatchCompute(numParticles / local_size.x + 1, 1, 1);
}
void G2P_global::dispatch_with_barrier(UniformsDynamic&& uniforms) {
  dispatch(std::move(uniforms));
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}
void G2P_global::uniforms_update(UniformsDynamic&& uniforms) {
  Technique::uniform_update("indexSize", uniforms.numParticles);
}

