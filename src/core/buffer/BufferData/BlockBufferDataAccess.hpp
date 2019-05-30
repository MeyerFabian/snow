#ifndef BLOCKBUFFERDATAACCESS_HPP_HQMWAVIY
#define BLOCKBUFFERDATAACCESS_HPP_HQMWAVIY
#include <memory>
#include "../../shader/shader.hpp"
#include "../../utils/string_to_upper.hpp"
#include "../buffer.hpp"
#include "BlockBufferData.hpp"
#include "BufferDataInterface.hpp"

/*
 * BlockBufferDataAccess can be requested from the BlockPipeline s.t. other
 * shaders can use the results to get the maximum cell number within a shared
 * memory block
 *
 * results in:
 * #define OUTPUT_BLOCK_COUNT blocks
 * #define OUTPUT_BLOCK_COUNT_VAR Block_counter
 * #define OUTPUT_BLOCK_COUNT_SIZE 16384
 * #define OUTPUT_BLOCK_COUNT_VAR_SIZE Block_VAR_SIZE
 * #define OUTPUT_BLOCK_COUNT_NUM_BUFFER 1
 * #define OUTPUT_BLOCK_COUNT_INDEX_BUFFER 0
 * #define OUTPUT_BLOCK_COUNT_AT SOA
 */
class BlockBufferDataAccess : public BufferDataInterface {
 public:
  struct IndexSSBOData {
    std::unique_ptr<BufferDataInterface> count;
  };

  BlockBufferDataAccess(std::unique_ptr<BlockBufferData> in_buffer,
                        IndexSSBOData&& in_ssbo)
      : block_buffer(std::move(in_buffer)), ssbo(std::move(in_ssbo)) {}

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

  std::unique_ptr<BlockBufferData> block_buffer;
  IndexSSBOData ssbo;
};

#endif /* end of include guard: BLOCKBUFFERDATAACCESS_HPP_HQMWAVIY */

