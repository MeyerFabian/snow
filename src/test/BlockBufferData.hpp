#ifndef BLOCKBUFFERDATA_HPP_RIQXSHUS
#define BLOCKBUFFERDATA_HPP_RIQXSHUS
#include "BufferData.hpp"
class BlockBufferData : public BufferDataInterface {
 public:
  struct IndexSSBOData {
    std::unique_ptr<BufferDataInterface> index;
  };

  BlockBufferData(std::unique_ptr<BufferDataInterface> in_buffer,
                  IndexSSBOData&& in_ssbo)
      : buffer_interface(std::move(in_buffer)), ssbo(std::move(in_ssbo)) {}

  virtual std::unique_ptr<BufferDataInterface> cloneBufferDataInterface()
      override;

  virtual std::vector<Shader::CommandType> generateCommands(
      bool, std::string) override;

  void setName(std::string name) override;
  std::string getName() override;

  void setVariable(std::string name) override;
  std::string getVariable() override;

  void setIndexBuffer(std::string) override;
  std::string getIndexBuffer() override;

  GLuint getSize() override;

  std::unique_ptr<BlockBufferData> clone();
  std::unique_ptr<BufferDataInterface> buffer_interface;
  IndexSSBOData ssbo;
};

#endif /* end of include guard: BLOCKBUFFERDATA_HPP_RIQXSHUS */

