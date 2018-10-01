#ifndef IOBUFFERDATA_HPP_FHDBM2YC
#define IOBUFFERDATA_HPP_FHDBM2YC
#include "../snow/buffer/buffer.hpp"
#include "../snow/shader/shader.hpp"

struct IOBufferData {
  struct Data {
    std::string name;
    std::string var;
    BufferInfo info;
  };
  Data in_buffer;
  Data out_buffer;

  auto generateCommands() {
    std::vector<Shader::CommandType> vec = {
        {PreprocessorCmd::DEFINE, "INPUT " + in_buffer.name},
        {PreprocessorCmd::DEFINE, "OUTPUT " + out_buffer.name},
        {PreprocessorCmd::DEFINE, "INPUT_VAR " + in_buffer.var},
        {PreprocessorCmd::DEFINE, "OUTPUT_VAR " + out_buffer.var},
        {PreprocessorCmd::INCLUDE, "\"" + in_buffer.info.glsl_filename + "\""},
        {PreprocessorCmd::INCLUDE,
         "\"" + out_buffer.info.glsl_filename + "\""}};
    return vec;
  }
};

#endif /* end of include guard: IOBUFFERDATA_HPP_FHDBM2YC */

