#ifndef IOBUFFERDATA_HPP_FHDBM2YC
#define IOBUFFERDATA_HPP_FHDBM2YC
#include "../snow/buffer/buffer.hpp"
#include "../snow/shader/shader.hpp"

struct IOBufferDataInterface {
  virtual std::vector<Shader::CommandType> generateCommands() = 0;
  virtual BufferLayout getLayout() = 0;
};
struct Data {
  std::string name;
  std::string var;
  BufferInfo info;
  GLuint bufferSize;
  GLuint bufferNum = 1;
};
struct IOBufferData : public IOBufferDataInterface {
  IOBufferData(Data in, Data out) : in_buffer(in), out_buffer(out){};

  Data in_buffer;
  Data out_buffer;
  std::vector<Shader::CommandType> generateCommands() {
    std::vector<Shader::CommandType> vec = {
        {PreprocessorCmd::DEFINE, "INPUT " + in_buffer.name},
        {PreprocessorCmd::DEFINE, "INPUT_VAR " + in_buffer.var},
        {PreprocessorCmd::DEFINE,
         std::string("INPUT_SIZE ") + std::to_string(in_buffer.bufferSize)},
        {PreprocessorCmd::DEFINE, std::string("INPUT_NUM_BUFFER ") +
                                      std::to_string(in_buffer.bufferNum)},
        {PreprocessorCmd::INCLUDE, "\"" + in_buffer.info.glsl_filename + "\""},

        {PreprocessorCmd::DEFINE, "OUTPUT " + out_buffer.name},
        {PreprocessorCmd::DEFINE, "OUTPUT_VAR " + out_buffer.var},
        {PreprocessorCmd::DEFINE,
         std::string("OUTPUT_SIZE ") + std::to_string(out_buffer.bufferSize)},
        {PreprocessorCmd::DEFINE, std::string("OUTPUT_NUM_BUFFER ") +
                                      std::to_string(out_buffer.bufferNum)},

        {PreprocessorCmd::INCLUDE,
         "\"" + out_buffer.info.glsl_filename + "\""}};
    return vec;
  }

  BufferLayout getLayout() { return in_buffer.info.layout; }
};

struct IO2BufferData : public IOBufferDataInterface {
  IO2BufferData(Data in, Data out, Data out2)
      : io({in, out}), out2_buffer(out2){};
  IOBufferData io;
  Data out2_buffer;

  std::vector<Shader::CommandType> generateCommands() {
    std::vector<Shader::CommandType> vec = io.generateCommands();
    vec.insert(vec.end(),
               {
                   {PreprocessorCmd::DEFINE, "OUTPUT2 " + out2_buffer.name},
                   {PreprocessorCmd::DEFINE, "OUTPUT2_VAR " + out2_buffer.var},

                   {PreprocessorCmd::DEFINE,
                    std::string("OUTPUT2_SIZE ") +
                        std::to_string(out2_buffer.bufferSize)},
                   {PreprocessorCmd::DEFINE,
                    std::string("OUTPUT2_NUM_BUFFER ") +
                        std::to_string(out2_buffer.bufferNum)},
                   {PreprocessorCmd::INCLUDE,
                    "\"" + out2_buffer.info.glsl_filename + "\""},
               });
    return vec;
  }

  BufferLayout getLayout() { return io.getLayout(); }
};

#endif /* end of include guard: IOBUFFERDATA_HPP_FHDBM2YC */

