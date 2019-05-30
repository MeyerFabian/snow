#include "BlockBufferDataAccess.hpp"
void BlockBufferDataAccess::setName(std::string name) {
  block_buffer->setName(name);
}
std::string BlockBufferDataAccess::getName() { return block_buffer->getName(); }

void BlockBufferDataAccess::setIndexBuffer(std::string in_index) {
  block_buffer->setIndexBuffer(in_index);
}
std::string BlockBufferDataAccess::getIndexBuffer() {
  return block_buffer->getIndexBuffer();
}

void BlockBufferDataAccess::setVariable(std::string name) {
  block_buffer->setVariable(name);
}
std::string BlockBufferDataAccess::getVariable() {
  return block_buffer->getVariable();
}

GLuint BlockBufferDataAccess::getSize() { return block_buffer->getSize(); }

std::vector<Shader::CommandType> BlockBufferDataAccess::generateCommands(
    bool abstract, std::string define_name) {
  auto vec = block_buffer->generateCommands(abstract, define_name);

  std::string ssbo_define_count = define_name + "_BLOCK_COUNT";
  auto vec_ssbo_count =
      ssbo.count->generateCommands(abstract, ssbo_define_count);
  vec.insert(vec.end(), vec_ssbo_count.begin(), vec_ssbo_count.end());

  return vec;
}
std::unique_ptr<BufferDataInterface>
BlockBufferDataAccess::cloneBufferDataInterface() {
  return std::make_unique<BlockBufferDataAccess>(
      block_buffer->clone(), IndexSSBOData{
                                 ssbo.count->cloneBufferDataInterface(),
                             });
}

