#include "binTechnique.hpp"
void BinningTechnique::init(BinningData&& data, IOBufferData&& io) {
  auto shader = std::make_shared<Shader>(ShaderType::COMPUTE, data.filename);

  multiple = data.multiple;
  multiple_elements = data.multiple_elements;
  shader->set_local_size(local_size);

  std::vector<Shader::CommandType> vec = {
      {PreprocessorCmd::DEFINE,
       "MULTIPLE_ELEMENTS " + std::to_string(data.multiple_elements)},
  };

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
  GLuint dispatchSize = numVectors / (multiple_elements);
  Technique::uniform_update("bufferSize", numVectors);
  if (multiple) {
    Technique::uniform_update("dispatchSize", dispatchSize);
  }
  glDispatchCompute(dispatchSize / local_size.x + 1, 1, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

