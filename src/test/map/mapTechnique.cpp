#include "mapTechnique.hpp"
void MapTechnique::init(MapData&& data, IOBufferData&& io) {
  auto shader = std::make_shared<Shader>(ShaderType::COMPUTE, data.filename);

  shader->set_local_size(local_size);

  std::vector<Shader::CommandType> vec = {
      {PreprocessorCmd::DEFINE, "UNARY_OP(value) " + data.gl_unary_op},
  };
  auto io_cmds(io.generateCommands());
  vec.insert(std::end(vec), std::begin(io_cmds), std::end(io_cmds));

  shader->add_cmds(vec.begin(), vec.end());

  Technique::add_shader(std::move(shader));
  Technique::upload();
  Technique::use();
}
void MapTechnique::uniform_update(UniformsDynamic&& data) {
  Technique::uniform_update("bufferSize", data.bufferSize);
  if (multiple) {
    Technique::uniform_update("dispatchDim_x", data.dispatchDim_x);
  }
}
void MapTechnique::dispatch(DispatchData&& data) {
  Technique::use();
  multiple = data.multiple;
  GLuint dispatchDim_x = data.bufferSize / (local_size.x * data.seq_elements);
  UniformsDynamic uniforms{
      data.bufferSize,
      dispatchDim_x,
  };
  uniform_update(std::move(uniforms));
  glDispatchCompute(dispatchDim_x + 1, 1, 1);
}
void MapTechnique::dispatch_with_barrier(DispatchData&& data) {
  dispatch(std::move(data));
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

