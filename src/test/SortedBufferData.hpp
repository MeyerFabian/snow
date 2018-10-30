#ifndef SORTEDBUFFERDATA_HPP_HQMWAVIY
#define SORTEDBUFFERDATA_HPP_HQMWAVIY
#include <memory>
#include "../snow/buffer/buffer.hpp"
#include "../snow/shader/shader.hpp"
#include "BufferDataInterface.hpp"

enum class SortingMethod { Full, Index };

class SortedBufferData : public BufferDataInterface {
 public:
  SortedBufferData(SortingMethod in_sort,
                   std::unique_ptr<BufferDataInterface> in_buffer)

      : buffer_interface(std::move(in_buffer)), sorting_method(in_sort) {}

  std::unique_ptr<BufferDataInterface> buffer_interface;

  SortingMethod sorting_method;
  std::vector<Shader::CommandType> generateCommands(bool, std::string) override;

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

