#include "mapTechnique.hpp"
void MapTechnique::init(MapData&& data) {
  auto shader = std::make_shared<Shader>(ShaderType::COMPUTE,
                                         "shader/compute/mapreduce/map.glsl");

  shader->add_n_define(data.numVectors);
  shader->set_local_size(local_size);

  std::vector<Shader::CommandType> vec = {
      {PreprocessorCmd::DEFINE, "UNARY_OP(value) " + data.gl_unary_op},
      {PreprocessorCmd::DEFINE, "INPUT " + data.input},
      {PreprocessorCmd::DEFINE, "OUTPUT " + data.output},
      {PreprocessorCmd::DEFINE, "INPUT_VAR " + data.input_var},
      {PreprocessorCmd::DEFINE, "OUTPUT_VAR " + data.output_var},
      {PreprocessorCmd::INCLUDE, "\"" + data.buffer_filename + "\""}};

  shader->add_cmds(vec.begin(), vec.end());

  Technique::add_shader(std::move(shader));
  Technique::upload();
  Technique::use();
}

