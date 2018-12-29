#include "streamCompactionTechnique.hpp"
void StreamCompactionTechnique::init(StreamCompactionData&& data,
                                     IOBufferData&& io) {
  local_size = data.local_size;
  auto shader = std::make_shared<Shader>(
      ShaderType::COMPUTE, "shader/compute/preprocess/stream_compaction.glsl");

  shader->set_local_size(local_size);

  std::vector<Shader::CommandType> vec = {
      {PreprocessorCmd::DEFINE, "ABSTRACT_LAYOUT"},
      {PreprocessorCmd::DEFINE, "UNARY_OP(value) " + data.gl_unary_op},
  };
  auto io_cmds(io.generateCommands(true));
  vec.insert(std::end(vec), std::begin(io_cmds), std::end(io_cmds));

  shader->add_cmds(vec.begin(), vec.end());

  Technique::add_shader(std::move(shader));
  Technique::upload();
  Technique::use();
}
void StreamCompactionTechnique::dispatch_with_barrier(
    DispatchData&& data) const {
  Technique::use();
  uniforms_update({data.bufferSize});
  glDispatchCompute(data.bufferSize / local_size.x + 1, 1, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}
void StreamCompactionTechnique::uniforms_update(
    UniformsDynamic&& uniforms) const {
  Technique::uniform_update("bufferSize", uniforms.bufferSize);
}
void StreamCompactionTechnique::uniforms_init(UniformsStatic&& uniforms) const {
}

