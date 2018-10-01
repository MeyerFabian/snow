#include "mapReduceTechnique.hpp"
void MapReduceTechnique::init(MapReduceData&& data) {
  local_size = data.local_size;

  auto shader =
      std::make_shared<Shader>(ShaderType::COMPUTE, data.shader_filename);

  shader->add_n_define(data.numVectors);
  shader->add_aos_define(data.io.in_buffer.info.layout);
  shader->set_local_size(data.local_size);

  std::vector<Shader::CommandType> vec = {
      {PreprocessorCmd::DEFINE, "UNARY_OP(value) " + data.gl_unary_op},
      {PreprocessorCmd::DEFINE, "BINARY_OP(left,right) " + data.gl_binary_op},
  };
  auto io_cmds(data.io.generateCommands());
  vec.insert(std::end(vec), std::begin(io_cmds), std::end(io_cmds));

  shader->add_cmds(vec.begin(), vec.end());

  Technique::add_shader(std::move(shader));
  Technique::upload();
  Technique::use();
}
void MapReduceTechnique::dispatch_with_barrier(GLuint numVectors) const {
  glDispatchCompute(numVectors / local_size.x, 1 / local_size.y,
                    1 / local_size.z);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}
void MapReduceTechnique::dispatch_with_barrier(DispatchData&& data) const {
  Technique::use();
  GLuint dispatchDim_x = data.dispatchDim_x;
  uniforms_update(std::move(data));
  glDispatchCompute(dispatchDim_x, 1, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}
void MapReduceTechnique::uniforms_update(DispatchData&& uniforms) const {
  Technique::uniform_update("dispatchDim_x", uniforms.dispatchDim_x);
  Technique::uniform_update("bufferSize", uniforms.bufferSize);
}
