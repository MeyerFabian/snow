#ifndef SORTEDINDEXREADBUFFERDATA_HPP_TPWNAZH9
#define SORTEDINDEXREADBUFFERDATA_HPP_TPWNAZH9

#include "BufferData.hpp"
#include "SortedBufferData.hpp"
class SortedIndexReadBufferData : public SortedBufferData {
 public:
  SortedIndexReadBufferData(std::unique_ptr<BufferDataInterface> in_buffer,
                            IndexSSBOData&& in_ssbo)

      : SortedBufferData(std::move(in_buffer)), ssbo(std::move(in_ssbo)) {}

  virtual std::vector<Shader::CommandType> generateCommands(
      bool, std::string) override;

  virtual std::unique_ptr<BufferDataInterface> cloneBufferDataInterface()
      override;

  virtual std::unique_ptr<SortedBufferData> clone() override;
  IndexSSBOData ssbo;
};

#endif /* end of include guard: SORTEDINDEXREADBUFFERDATA_HPP_TPWNAZH9 */

