#ifndef SORTEDINDEXWRITEBUFFERDATA_HPP_RIQXSHUS
#define SORTEDINDEXWRITEBUFFERDATA_HPP_RIQXSHUS
#include "SortedBufferData.hpp"
class SortedIndexWriteBufferData : public SortedBufferData {
 public:
  SortedIndexWriteBufferData(std::unique_ptr<BufferDataInterface> in_buffer,
                             IndexSSBOData&& in_ssbo, IndexUBOData&& in_ubo)

      : SortedBufferData(std::move(in_buffer)),
        ssbo(std::move(in_ssbo)),
        ubo(std::move(in_ubo)) {}

  virtual std::unique_ptr<BufferDataInterface> cloneBufferDataInterface()
      override;

  virtual std::vector<Shader::CommandType> generateCommands(
      bool, std::string) override;
  IndexSSBOData ssbo;
  IndexUBOData ubo;
};

#endif /* end of include guard: SORTEDINDEXWRITEBUFFERDATA_HPP_RIQXSHUS */

