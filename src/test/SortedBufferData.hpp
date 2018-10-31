#ifndef SORTEDBUFFERDATA_HPP_HQMWAVIY
#define SORTEDBUFFERDATA_HPP_HQMWAVIY
#include <memory>
#include "../snow/buffer/buffer.hpp"
#include "../snow/shader/shader.hpp"
#include "BufferDataInterface.hpp"

enum class SortingMethod { Full, IndexRead, IndexWrite };

class SortedBufferData : public BufferDataInterface {
 protected:
  SortedBufferData(std::unique_ptr<BufferDataInterface> in_buffer)

      : buffer_interface(std::move(in_buffer)) {}

  std::unique_ptr<BufferDataInterface> buffer_interface;

  virtual std::vector<Shader::CommandType> generateCommands(bool,
                                                            std::string) = 0;

  virtual std::unique_ptr<BufferDataInterface> cloneBufferDataInterface()
      const override;

  virtual void setName(std::string name) override;
  virtual std::string getName() override;

  virtual void setVariable(std::string name) override;
  virtual std::string getVariable() override;

  virtual void setIndexBuffer(std::string) override;
  virtual std::string getIndexBuffer() override;

  GLuint getSize() override;
};

#endif /* end of include guard: SORTEDBUFFERDATA_HPP_HQMWAVIY */

