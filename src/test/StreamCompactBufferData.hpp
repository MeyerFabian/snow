#ifndef STREAMCOMPACTBUFFERDATA_HPP_HQMWAVIY
#define STREAMCOMPACTBUFFERDATA_HPP_HQMWAVIY
#include <memory>
#include "../snow/buffer/buffer.hpp"
#include "../snow/shader/shader.hpp"
#include "../snow/utils/string_to_upper.hpp"
#include "BufferData.hpp"
#include "BufferDataInterface.hpp"
class StreamCompactBufferData : public BufferDataInterface {
 public:
  struct IndexSSBOData {
    BufferData data;
  };
  void setName(std::string name) override;
  std::string getName() override;

  void setVariable(std::string name) override;
  std::string getVariable() override;

  void setIndexBuffer(std::string) override;
  std::string getIndexBuffer() override;

  GLuint getSize() override;

  virtual std::unique_ptr<StreamCompactBufferData> clone() = 0;

  StreamCompactBufferData(std::unique_ptr<BufferDataInterface> in_buffer)
      : buffer_interface(std::move(in_buffer)) {}

  std::unique_ptr<BufferDataInterface> buffer_interface;
};

#endif /* end of include guard: STREAMCOMPACTBUFFERDATA_HPP_HQMWAVIY */

