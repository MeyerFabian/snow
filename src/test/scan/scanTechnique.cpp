#include "scanTechnique.hpp"
void ScanTechnique::init(ScanData&& data, IOBufferDataInterface&& io) {
  local_size = data.local_size;
  auto shader = std::make_shared<Shader>(ShaderType::COMPUTE, data.filename);

  shader->add_n_define(data.numVectors);
  shader->add_aos_define(io.getLayout());
  shader->set_local_size(local_size);

  std::string raking = std::to_string(data.raking);
  std::vector<Shader::CommandType> vec = {
      {PreprocessorCmd::DEFINE,
       "UNARY_OP_RETURN_TYPE " + data.gl_unary_op_return_type},
      {PreprocessorCmd::DEFINE, "UNARY_OP(value) " + data.gl_unary_op},
      {PreprocessorCmd::DEFINE, "BINARY_OP(left,right) " + data.gl_binary_op},
      {PreprocessorCmd::DEFINE,
       "BINARY_OP_NEUTRAL_ELEMENT " + data.gl_binary_op_neutral_elem},
      {PreprocessorCmd::DEFINE, "MULTIPLE_ELEMENTS " + raking},
  };
  auto io_cmds(io.generateCommands());
  vec.insert(std::end(vec), std::begin(io_cmds), std::end(io_cmds));

  shader->add_cmds(vec.begin(), vec.end());

  Technique::add_shader(std::move(shader));
  Technique::upload();
  Technique::use();
}
void ScanTechnique::dispatch_with_barrier(DispatchData&& data) const {
  Technique::use();
  GLuint dispatchDim_x = data.dispatchDim_x;
  uniforms_update(std::move(data));
  glDispatchCompute(dispatchDim_x, 1, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}
void ScanTechnique::uniforms_update(DispatchData&& uniforms) const {
  Technique::uniform_update("bufferSize", uniforms.bufferSize);
}

