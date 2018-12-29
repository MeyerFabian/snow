#include "BlockBufferData.hpp"
std::vector<Shader::CommandType> BlockBufferData::generateCommands(
    bool abstract, std::string define_name) {
  auto vec = buffer_interface->generateCommands(abstract, define_name);

  std::string ssbo_define_count = define_name + "_BLOCK_INDEX";
  auto vec_ssbo_index =
      ssbo.index->generateCommands(abstract, ssbo_define_count);
  vec.insert(vec.end(), vec_ssbo_index.begin(), vec_ssbo_index.end());

  return vec;
}

std::unique_ptr<BufferDataInterface>
BlockBufferData::cloneBufferDataInterface() {
  return std::make_unique<BlockBufferData>(
      buffer_interface->cloneBufferDataInterface(),
      IndexSSBOData{ssbo.index->cloneBufferDataInterface()});
}
std::unique_ptr<BlockBufferData> BlockBufferData::clone() {
  return std::make_unique<BlockBufferData>(
      buffer_interface->cloneBufferDataInterface(),
      IndexSSBOData{ssbo.index->cloneBufferDataInterface()});
}

void BlockBufferData::setName(std::string name) {
  buffer_interface->setName(name);
}
std::string BlockBufferData::getName() { return buffer_interface->getName(); }

void BlockBufferData::setIndexBuffer(std::string in_index) {
  buffer_interface->setIndexBuffer(in_index);
}
std::string BlockBufferData::getIndexBuffer() {
  return buffer_interface->getIndexBuffer();
}

void BlockBufferData::setVariable(std::string name) {
  buffer_interface->setVariable(name);
}
std::string BlockBufferData::getVariable() {
  return buffer_interface->getVariable();
}

GLuint BlockBufferData::getSize() { return buffer_interface->getSize(); }

