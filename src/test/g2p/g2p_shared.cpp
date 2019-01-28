#include "g2p_shared.hpp"

void G2P_shared::init(G2PData&& data, IOBufferData&& io) {
  auto shader = std::make_shared<Shader>(ShaderType::COMPUTE, filename);

  block_dispatch = std::move(data.block_indirect);

  shader->set_local_size(local_size);

  auto io_cmds(io.generateCommands());

  if (data.apic) {
    vec.insert(std::end(vec), {PreprocessorCmd::DEFINE, "APIC"});
  }

  vec.insert(std::end(vec), std::begin(io_cmds), std::end(io_cmds));

  shader->add_cmds(vec.begin(), vec.end());

  Technique::add_shader(std::move(shader));
  Technique::upload();
  Technique::use();
  uniforms_init(std::move(data.uniforms));
}
void G2P_shared::init_pull(G2PData&& data, IOBufferData&& io) {
  filename = "shader/test/shared/g2p_shared.glsl";
  init(std::move(data), std::move(io));
}
void G2P_shared::init_pull_batching(G2PBatchingData&& data, IOBufferData&& io) {
  std::string multiple_particles = std::to_string(data.multiple_particles);
  vec.push_back(
      {PreprocessorCmd::DEFINE, "MULTIPLE_PARTICLES " + multiple_particles});
  filename = "shader/test/shared/g2p_shared_multiple.glsl";
  init(std::move(data.g2p_data), std::move(io));
}
void G2P_shared::uniforms_init(UniformsStatic&& uniforms) {
  gGridDim = uniforms.gGridDim;
}
void G2P_shared::dispatch(UniformsDynamic&& uniforms) {
  Technique::use();
  uniforms_update(std::move(uniforms));
  if (*block_dispatch) {
    (*block_dispatch)->indirect_dispatch();
  } else {
    glDispatchCompute(gGridDim.x * gGridDim.y * gGridDim.z / (local_size.x), 1,
                      1);
  }
}
void G2P_shared::dispatch_with_barrier(UniformsDynamic&& uniforms) {
  dispatch(std::move(uniforms));
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}
void G2P_shared::uniforms_update(UniformsDynamic&& uniforms) {}

