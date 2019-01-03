#include "p2g_shared.hpp"

void P2G_shared::init(P2GData&& data, IOBufferData&& io) {
  auto shader = std::make_shared<Shader>(ShaderType::COMPUTE, filename);

  block_dispatch = std::move(data.block_indirect);

  shader->set_local_size(local_size);

  auto io_cmds(io.generateCommands());
  vec.insert(std::end(vec), std::begin(io_cmds), std::end(io_cmds));

  shader->add_cmds(vec.begin(), vec.end());

  Technique::add_shader(std::move(shader));
  Technique::upload();
  Technique::use();
  uniforms_init(std::move(data.uniforms));
}
void P2G_shared::init_atomic(P2GData&& data, IOBufferData&& io) {
  filename = "shader/test/shared/p2g_atomic.glsl";
  init(std::move(data), std::move(io));
}
void P2G_shared::init_sync(P2GData&& data, IOBufferData&& io) {
  filename = "shader/test/shared/p2g_sync.glsl";
  init(std::move(data), std::move(io));
}
void P2G_shared::init_pull_simple(P2GData&& data, IOBufferData&& io) {
  filename = "shader/test/shared/p2g_pull_simple.glsl";
  init(std::move(data), std::move(io));
}
void P2G_shared::init_atomic_loop_reverse(P2GData&& data, IOBufferData&& io) {
  filename = "shader/test/shared/p2g_atomic_loop_reverse.glsl";
  init(std::move(data), std::move(io));
}
void P2G_shared::init_atomic_batching(P2GBatchingData&& data,
                                      IOBufferData&& io) {
  std::string multiple_particles = std::to_string(data.multiple_particles);
  vec.push_back(
      {PreprocessorCmd::DEFINE, "MULTIPLE_PARTICLES " + multiple_particles});
  filename = "shader/test/shared/p2g_atomic_batching.glsl";
  init(std::move(data.p2g_data), std::move(io));
}
void P2G_shared::init_sync_batching(P2GBatchingData&& data, IOBufferData&& io) {
  std::string multiple_particles = std::to_string(data.multiple_particles);
  vec.push_back(
      {PreprocessorCmd::DEFINE, "MULTIPLE_PARTICLES " + multiple_particles});
  filename = "shader/test/shared/p2g_sync_batching.glsl";
  init(std::move(data.p2g_data), std::move(io));
}
void P2G_shared::uniforms_init(UniformsStatic&& uniforms) {
  gGridDim = uniforms.gGridDim;
}
void P2G_shared::dispatch(UniformsDynamic&& uniforms) {
  Technique::use();
  uniforms_update(std::move(uniforms));
  if (*block_dispatch) {
    (*block_dispatch)->indirect_dispatch();
  } else {
    glDispatchCompute(gGridDim.x * gGridDim.y * gGridDim.z / (local_size.x), 1,
                      1);
  }
}
void P2G_shared::dispatch_with_barrier(UniformsDynamic&& uniforms) {
  dispatch(std::move(uniforms));
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}
void P2G_shared::uniforms_update(UniformsDynamic&& uniforms) {}

