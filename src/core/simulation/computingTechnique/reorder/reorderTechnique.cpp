#include "reorderTechnique.hpp"
/*
 * Abstract layout assumes that all the data has the same datatype and is either
 * SoA or AoS. It then just reorders everything without knowing what the
 * variables represent.
 */
void ReorderTechnique::init(ReorderData&& data, IOBufferData&& io) {
  local_size = data.local_size;
  dw_back = data.dw_back;
  auto shader = std::make_shared<Shader>(ShaderType::COMPUTE, data.filename);

  shader->set_local_size(local_size);

  std::vector<Shader::CommandType> vec = {
      {PreprocessorCmd::DEFINE, "ABSTRACT_LAYOUT"},
  };
  if (dw_back) {
    vec.insert(std::end(vec),
               {PreprocessorCmd::DEFINE, "SCAN_DIRECT_WRITE_BACK"});
  }
  auto io_cmds(io.generateCommands(true));
  vec.insert(std::end(vec), std::begin(io_cmds), std::end(io_cmds));

  shader->add_cmds(vec.begin(), vec.end());

  Technique::add_shader(std::move(shader));
  Technique::upload();
  Technique::use();
  uniforms_init({data.scan_block_size});
}

// expects data.bufferSize to be global
void ReorderTechnique::dispatch_with_barrier(DispatchData&& data) const {
  Technique::use();
  uniforms_update({data.bufferSize});
  glDispatchCompute(data.bufferSize / local_size.x + 1, 1, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void ReorderTechnique::uniforms_update(UniformsDynamic&& uniforms) const {
  Technique::uniform_update("bufferSize", uniforms.bufferSize);
}

void ReorderTechnique::uniforms_init(UniformsStatic&& uniforms) const {
  if (!dw_back) {
    Technique::uniform_update("scanBlockSize", uniforms.scanBlockSize);
  }
}

