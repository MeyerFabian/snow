#include "scanTechnique.hpp"
void ScanTechnique::init(ScanData&& data) {
  auto shader = std::make_shared<Shader>(ShaderType::COMPUTE,
                                         "shader/compute/preprocess/scan.glsl");

  shader->add_n_define(data.numVectors);
  shader->add_aos_define(data.io.in_buffer.info.layout);
  shader->set_local_size(local_size);

  std::vector<Shader::CommandType> vec = {
      {PreprocessorCmd::DEFINE,
       "UNARY_OP_RETURN_TYPE " + data.gl_unary_op_return_type},
      {PreprocessorCmd::DEFINE, "UNARY_OP(value) " + data.gl_unary_op},
      {PreprocessorCmd::DEFINE, "BINARY_OP(left,right) " + data.gl_binary_op},
      {PreprocessorCmd::DEFINE,
       "BINARY_OP_NEUTRAL_ELEMENT " + data.gl_binary_op_neutral_elem},

  };
  auto io_cmds(data.io.generateCommands());
  vec.insert(std::end(vec), std::begin(io_cmds), std::end(io_cmds));

  shader->add_cmds(vec.begin(), vec.end());

  Technique::add_shader(std::move(shader));
  Technique::upload();
  Technique::use();
}
void ScanTechnique::dispatch_with_barrier(GLuint numVectors) {
  Technique::use();
  Technique::uniform_update("bufferSize", numVectors);
  glDispatchCompute(numVectors / local_size.x / 2, 1 / local_size.y,
                    1 / local_size.z);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

