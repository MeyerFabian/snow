#ifndef STREAMCOMPACTINDEXBUFFERDATA_HPP_TPWNAZH9
#define STREAMCOMPACTINDEXBUFFERDATA_HPP_TPWNAZH9

#include "BufferData.hpp"
#include "StreamCompactBufferData.hpp"
class StreamCompactIndexBufferData : public StreamCompactBufferData {
 public:
  StreamCompactIndexBufferData(std::unique_ptr<BufferDataInterface> in_buffer,
                               IndexSSBOData&& in_ssbo)

      : StreamCompactBufferData(std::move(in_buffer)),
        ssbo(std::move(in_ssbo)) {}

  virtual std::vector<Shader::CommandType> generateCommands(
      bool, std::string) override;

  virtual std::unique_ptr<BufferDataInterface> cloneBufferDataInterface()
      override;

  virtual std::unique_ptr<StreamCompactBufferData> clone() override;
  IndexSSBOData ssbo;
};

#endif /* end of include guard: STREAMCOMPACTINDEXBUFFERDATA_HPP_TPWNAZH9 */

