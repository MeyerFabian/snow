#ifndef SORTEDINDEXWRITEBUFFERDATA_HPP_RIQXSHUS
#define SORTEDINDEXWRITEBUFFERDATA_HPP_RIQXSHUS
#include "SortedBufferData.hpp"
class SortedIndexWriteBufferData : public SortedBufferData {
 public:
  SortedIndexWriteBufferData(std::unique_ptr<BufferDataInterface> in_buffer,
                             IndexSSBOData&& in_ssbo, IndexUBOData&& in_ubo,
                             std::string in_sorting_key)

      : SortedBufferData(std::move(in_buffer)),
        ssbo(std::move(in_ssbo)),
        ubo(std::move(in_ubo)),
        sorting_key(in_sorting_key) {}

  virtual std::unique_ptr<BufferDataInterface> cloneBufferDataInterface()
      override;

  virtual std::unique_ptr<SortedBufferData> clone() override;
  virtual std::vector<Shader::CommandType> generateCommands(
      bool, std::string) override;
  IndexSSBOData ssbo;
  IndexUBOData ubo;
  std::string sorting_key;
};

#endif /* end of include guard: SORTEDINDEXWRITEBUFFERDATA_HPP_RIQXSHUS */

