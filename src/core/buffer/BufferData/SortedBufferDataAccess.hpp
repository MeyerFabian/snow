#ifndef SORTEDBUFFERDATAACCESS_HPP_HQMWAVIY
#define SORTEDBUFFERDATAACCESS_HPP_HQMWAVIY
#include <memory>
#include "../../buffer/buffer.hpp"
#include "../../shader/shader.hpp"
#include "../../utils/string_to_upper.hpp"
#include "BufferDataInterface.hpp"
#include "SortedBufferData.hpp"

/*
 * The Access class additionally exposes the scan and count which got calculated
 * by counting sort and binning respectively.
 */
class SortedBufferDataAccess : public BufferDataInterface {
 public:
  struct IndexSSBOData {
    std::unique_ptr<BufferDataInterface> scan;
    std::unique_ptr<BufferDataInterface> count;
  };

  SortedBufferDataAccess(std::unique_ptr<SortedBufferData> in_buffer,
                         IndexSSBOData&& in_ssbo)
      : sorted_buffer(std::move(in_buffer)), ssbo(std::move(in_ssbo)) {}

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

  std::unique_ptr<SortedBufferData> sorted_buffer;
  IndexSSBOData ssbo;
};

#endif /* end of include guard: SORTEDBUFFERDATAACCESS_HPP_HQMWAVIY */

