#ifndef IOBUFFERDATA_HPP_FHDBM2YC
#define IOBUFFERDATA_HPP_FHDBM2YC
#include "../snow/buffer/buffer.hpp"
#include "../snow/shader/shader.hpp"

struct BufferData {
  std::string name;
  std::string var;
  BufferInfo info;
  GLuint bufferSize;
  GLuint bufferIndex = 0;
  GLuint bufferNum = 1;
  std::string var_size_def = "1";
};

struct IOBufferData {
  IOBufferData(std::vector<BufferData> in, std::vector<BufferData> out)
      : in_buffer(in), out_buffer(out){};

  std::vector<BufferData> in_buffer;
  std::vector<BufferData> out_buffer;

  std::vector<Shader::CommandType> generateCommands();
  std::vector<Shader::CommandType> generateCommands(bool);
};
#endif /* end of include guard: IOBUFFERDATA_HPP_FHDBM2YC */

