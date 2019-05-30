#ifndef IOBUFFERDATA_HPP_FHDBM2YC
#define IOBUFFERDATA_HPP_FHDBM2YC
#include "../../buffer/buffer.hpp"
#include "../../shader/shader.hpp"
#include "BufferDataInterface.hpp"

// collects BufferDataInterfaces
struct IOBufferData {
  IOBufferData(std::vector<std::unique_ptr<BufferDataInterface> > in,
               std::vector<std::unique_ptr<BufferDataInterface> > out)
      : in_buffer(std::move(in)), out_buffer(std::move(out)){};
  IOBufferData() = default;

  std::vector<std::unique_ptr<BufferDataInterface> > in_buffer;
  std::vector<std::unique_ptr<BufferDataInterface> > out_buffer;

  std::vector<Shader::CommandType> generateCommands();
  std::vector<Shader::CommandType> generateCommands(bool);
  IOBufferData clone();
};
#endif /* end of include guard: IOBUFFERDATA_HPP_FHDBM2YC */

