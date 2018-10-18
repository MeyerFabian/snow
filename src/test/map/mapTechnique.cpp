#include "mapTechnique.hpp"
void MapTechnique::init(MapData&& data) {
  auto shader = std::make_shared<Shader>(ShaderType::COMPUTE,
                                         "shader/compute/mapreduce/map.glsl");

  shader->add_aos_define(data.io.in_buffer.info.layout);
  shader->set_local_size(local_size);

  std::vector<Shader::CommandType> vec = {
      {PreprocessorCmd::DEFINE, "UNARY_OP(value) " + data.gl_unary_op},
  };
  auto io_cmds(data.io.generateCommands());
  vec.insert(std::end(vec), std::begin(io_cmds), std::end(io_cmds));

  shader->add_cmds(vec.begin(), vec.end());

  Technique::add_shader(std::move(shader));
  Technique::upload();
  Technique::use();
}
void MapTechnique::dispatch_with_barrier(GLuint numVectors) {
  Technique::use();
  Technique::uniform_update("bufferSize", numVectors);
  glDispatchCompute(numVectors / local_size.x + 1, 1 / local_size.y,
                    1 / local_size.z);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

