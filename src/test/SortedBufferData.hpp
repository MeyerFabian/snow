#ifndef SORTEDBUFFERDATA_HPP_HQMWAVIY
#define SORTEDBUFFERDATA_HPP_HQMWAVIY
#include <memory>
#include "../snow/buffer/buffer.hpp"
#include "../snow/shader/shader.hpp"
#include "BufferDataInterface.hpp"

class SortedBufferData : public BufferDataInterface {
 public:
  void setName(std::string name) override;
  std::string getName() override;

  void setVariable(std::string name) override;
  std::string getVariable() override;

  void setIndexBuffer(std::string) override;
  std::string getIndexBuffer() override;

  GLuint getSize() override;
  SortedBufferData(std::unique_ptr<BufferDataInterface> in_buffer)
      : buffer_interface(std::move(in_buffer)) {}
  std::unique_ptr<BufferDataInterface> buffer_interface;
};

#endif /* end of include guard: SORTEDBUFFERDATA_HPP_HQMWAVIY */

