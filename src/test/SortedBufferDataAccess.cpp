
#include "SortedBufferDataAccess.hpp"
void SortedBufferDataAccess::setName(std::string name) {
  sorted_buffer->setName(name);
}
std::string SortedBufferDataAccess::getName() {
  return sorted_buffer->getName();
}

void SortedBufferDataAccess::setIndexBuffer(std::string in_index) {
  sorted_buffer->setIndexBuffer(in_index);
}
std::string SortedBufferDataAccess::getIndexBuffer() {
  return sorted_buffer->getIndexBuffer();
}

void SortedBufferDataAccess::setVariable(std::string name) {
  sorted_buffer->setVariable(name);
}
std::string SortedBufferDataAccess::getVariable() {
  return sorted_buffer->getVariable();
}

GLuint SortedBufferDataAccess::getSize() { return sorted_buffer->getSize(); }

std::vector<Shader::CommandType> SortedBufferDataAccess::generateCommands(
    bool abstract, std::string define_name) {
  auto vec = sorted_buffer->generateCommands(abstract, define_name);

  std::string ssbo_define_scan = define_name + "_SCAN";
  std::string ssbo_define_count = define_name + "_COUNT";
  auto vec_ssbo_scan = ssbo.scan.generateCommands(abstract, ssbo_define_scan);
  auto vec_ssbo_count =
      ssbo.count.generateCommands(abstract, ssbo_define_count);
  vec.insert(vec.end(), vec_ssbo_scan.begin(), vec_ssbo_scan.end());
  vec.insert(vec.end(), vec_ssbo_count.begin(), vec_ssbo_count.end());

  return vec;
}
std::unique_ptr<BufferDataInterface>
SortedBufferDataAccess::cloneBufferDataInterface() {
  return std::make_unique<SortedBufferDataAccess>(
      sorted_buffer->cloneBufferDataInterface(), IndexSSBOData(ssbo));
}

