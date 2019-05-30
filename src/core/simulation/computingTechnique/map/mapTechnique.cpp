#include "mapTechnique.hpp"
/*
 * IOBufferData expects:
 * in_buffer [0]: buffer to apply UNARY_OP on
 * out_buffer[0]: buffer to store mapped data in
 */
void MapTechnique::init(MapData&& data, IOBufferData&& io) {
  auto shader = std::make_shared<Shader>(ShaderType::COMPUTE, data.filename);

  if (data.local_size) local_size = *data.local_size;
  shader->set_local_size(local_size);

  std::vector<Shader::CommandType> vec = {
      {PreprocessorCmd::DEFINE, "UNARY_OP(value) " + data.gl_unary_op},
  };

  commands.insert(std::end(commands), std::begin(vec), std::end(vec));
  auto io_cmds(io.generateCommands());
  commands.insert(std::end(commands), std::begin(io_cmds), std::end(io_cmds));

  shader->add_cmds(commands.begin(), commands.end());

  Technique::add_shader(std::move(shader));
  Technique::upload();
  Technique::use();
}

/*
 * IOBufferData expects:
 * in_buffer [0]: buffer to apply UNARY_OP on
 * out_buffer[0]: buffer to store mapped data in
 */
void MapTechnique::init(std::vector<Shader::CommandType>&& in_cmds,
                        MapData&& data, IOBufferData&& io) {
  auto shader = std::make_shared<Shader>(ShaderType::COMPUTE, data.filename);

  if (data.local_size) local_size = *data.local_size;
  shader->set_local_size(local_size);

  std::vector<Shader::CommandType> vec = {
      {PreprocessorCmd::DEFINE, "UNARY_OP(value) " + data.gl_unary_op},
  };

  commands.insert(std::end(commands), std::begin(vec), std::end(vec));
  auto io_cmds(io.generateCommands());
  commands.insert(std::end(commands), std::begin(io_cmds), std::end(io_cmds));

  commands.insert(std::end(commands), std::begin(in_cmds), std::end(in_cmds));

  shader->add_cmds(commands.begin(), commands.end());

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
/*
 * @data.bufferSize: expects global size
 */
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

