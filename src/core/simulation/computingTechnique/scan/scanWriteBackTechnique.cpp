#include "scanWriteBackTechnique.hpp"
/*
 * @data.scanBlockSize: scan block size to know how much to spread in write back
 * @data.gl_binary_op : needs to do same binary operation as scan
 */
void ScanWriteBackTechnique::init(ScanWriteBackData&& data, IOBufferData&& io) {
  auto shader = std::make_shared<Shader>(
      ShaderType::COMPUTE, "shader/compute/scan/scan_write_back.glsl");

  shader->set_local_size(local_size);

  std::vector<Shader::CommandType> vec = {
      {PreprocessorCmd::DEFINE,
       "scanBlockSize " + std::to_string(data.scanBlockSize)},
      {PreprocessorCmd::DEFINE, "BINARY_OP(left,right) " + data.gl_binary_op},
  };
  auto io_cmds(io.generateCommands());
  vec.insert(std::end(vec), std::begin(io_cmds), std::end(io_cmds));

  shader->add_cmds(vec.begin(), vec.end());

  Technique::add_shader(std::move(shader));
  Technique::upload();
  Technique::use();
}

// @numVectors: expects global size
void ScanWriteBackTechnique::dispatch_with_barrier(GLuint numVectors) const {
  Technique::use();
  glDispatchCompute(numVectors / local_size.x, 1, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

