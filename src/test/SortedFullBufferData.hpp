#ifndef SORTEDFULLBUFFERDATA_HPP_RIQXSHUS
#define SORTEDFULLBUFFERDATA_HPP_RIQXSHUS

#include "BufferData.hpp"
#include "SortedBufferData.hpp"
class SortedFullBufferData : public SortedBufferData {
 public:
  SortedFullBufferData(std::unique_ptr<BufferDataInterface> in_buffer,
                       IndexUBOData&& in_ubo)
      : SortedBufferData(std::move(in_buffer)), ubo(std::move(in_ubo)) {}

  virtual std::unique_ptr<BufferDataInterface> cloneBufferDataInterface()
      override;

  virtual std::vector<Shader::CommandType> generateCommands(
      bool, std::string) override;
  IndexUBOData ubo;
};

#endif /* end of include guard: SORTEDFULLBUFFERDATA_HPP_RIQXSHUS */

