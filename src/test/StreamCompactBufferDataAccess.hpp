
#ifndef STREAMCOMPACTBUFFERDATAACCESS_HPP_HQMWAVIY
#define STREAMCOMPACTBUFFERDATAACCESS_HPP_HQMWAVIY
#include <memory>
#include "../snow/buffer/buffer.hpp"
#include "../snow/shader/shader.hpp"
#include "../snow/utils/string_to_upper.hpp"
#include "BufferDataInterface.hpp"
#include "StreamCompactBufferData.hpp"
class StreamCompactBufferDataAccess : public BufferDataInterface {
 public:
  struct IndexSSBOData {
    std::unique_ptr<BufferDataInterface> scan;
    std::unique_ptr<BufferDataInterface> count;
  };

  StreamCompactBufferDataAccess(
      std::unique_ptr<StreamCompactBufferData> in_buffer,
      IndexSSBOData&& in_ssbo)
      : stream_compact_buffer(std::move(in_buffer)), ssbo(std::move(in_ssbo)) {}

  void setName(std::string name) override;
  std::string getName() override;

  void setVariable(std::string name) override;
  std::string getVariable() override;

  void setIndexBuffer(std::string) override;
  std::string getIndexBuffer() override;

  GLuint getSize() override;
  virtual std::vector<Shader::CommandType> generateCommands(
      bool, std::string) override;

  virtual std::unique_ptr<BufferDataInterface> cloneBufferDataInterface()
      override;

  std::unique_ptr<StreamCompactBufferData> stream_compact_buffer;
  IndexSSBOData ssbo;
};

#endif /* end of include guard: STREAMCOMPACTBUFFERDATAACCESS_HPP_HQMWAVIY */

