#include "reorderTechnique.hpp"
void ReorderTechnique::init(ReorderData&& data, IOBufferData&& io) {
  local_size = data.local_size;
  auto shader = std::make_shared<Shader>(ShaderType::COMPUTE, data.filename);

  shader->set_local_size(local_size);

  std::vector<Shader::CommandType> vec = {
      {PreprocessorCmd::DEFINE, "ABSTRACT_LAYOUT"},
  };
  auto io_cmds(io.generateCommands(true));
  vec.insert(std::end(vec), std::begin(io_cmds), std::end(io_cmds));

  shader->add_cmds(vec.begin(), vec.end());

  Technique::add_shader(std::move(shader));
  Technique::upload();
  Technique::use();
}
void ReorderTechnique::dispatch_with_barrier(DispatchData&& data) const {
  Technique::use();
  GLuint dispatchDim_x = data.dispatchDim_x;
  uniforms_update(std::move(data));
  glDispatchCompute(dispatchDim_x, 1, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}
void ReorderTechnique::uniforms_update(DispatchData&& uniforms) const {
  Technique::uniform_update("bufferSize", uniforms.bufferSize);
}

