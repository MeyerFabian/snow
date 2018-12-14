#ifndef SORTEDFULLBUFFERDATA_HPP_RIQXSHUS
#define SORTEDFULLBUFFERDATA_HPP_RIQXSHUS

#include "BufferData.hpp"
#include "SortedBufferData.hpp"
class SortedFullBufferData : public SortedBufferData {
 public:
  SortedFullBufferData(std::unique_ptr<BufferDataInterface> in_buffer,
                       IndexUBOData&& in_ubo, std::string in_sorting_key)
      : SortedBufferData(std::move(in_buffer)),
        ubo(std::move(in_ubo)),
        sorting_key(in_sorting_key) {}

  virtual std::unique_ptr<BufferDataInterface> cloneBufferDataInterface()
      override;

  virtual std::vector<Shader::CommandType> generateCommands(
      bool, std::string) override;
  IndexUBOData ubo;
  std::string sorting_key;
};

#endif /* end of include guard: SORTEDFULLBUFFERDATA_HPP_RIQXSHUS */

