#include "binTechnique.hpp"
void BinningTechnique::init(BinningData&& data, IOBufferDataInterface&& io) {
  auto shader = std::make_shared<Shader>(ShaderType::COMPUTE,
                                         "shader/compute/preprocess/bin.glsl");

  shader->add_aos_define(io.getLayout());
  shader->set_local_size(local_size);

  std::vector<Shader::CommandType> vec = {};

  auto io_cmds(io.generateCommands());
  vec.insert(std::end(vec), std::begin(io_cmds), std::end(io_cmds));

  shader->add_cmds(vec.begin(), vec.end());

  Technique::add_shader(std::move(shader));
  Technique::upload();
  Technique::use();
  uniforms_init(std::move(data.uniforms));
}
void BinningTechnique::uniforms_init(UniformsStatic&& uniforms) {
  Technique::uniform_update("gGridPos", uniforms.gGridPos.x,
                            uniforms.gGridPos.y, uniforms.gGridPos.z);
  Technique::uniform_update("gGridDim", uniforms.gGridDim);
  Technique::uniform_update("gridSpacing", uniforms.gridSpacing);
}
void BinningTechnique::dispatch_with_barrier(GLuint numVectors) {
  Technique::use();
  Technique::uniform_update("bufferSize", numVectors);
  glDispatchCompute(numVectors / local_size.x + 1, 1 / local_size.y,
                    1 / local_size.z);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

