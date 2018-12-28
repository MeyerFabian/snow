#include "StreamCompactBufferDataAccess.hpp"
void StreamCompactBufferDataAccess::setName(std::string name) {
  stream_compact_buffer->setName(name);
}
std::string StreamCompactBufferDataAccess::getName() {
  return stream_compact_buffer->getName();
}

void StreamCompactBufferDataAccess::setIndexBuffer(std::string in_index) {
  stream_compact_buffer->setIndexBuffer(in_index);
}
std::string StreamCompactBufferDataAccess::getIndexBuffer() {
  return stream_compact_buffer->getIndexBuffer();
}

void StreamCompactBufferDataAccess::setVariable(std::string name) {
  stream_compact_buffer->setVariable(name);
}
std::string StreamCompactBufferDataAccess::getVariable() {
  return stream_compact_buffer->getVariable();
}

GLuint StreamCompactBufferDataAccess::getSize() {
  return stream_compact_buffer->getSize();
}

std::vector<Shader::CommandType>
StreamCompactBufferDataAccess::generateCommands(bool abstract,
                                                std::string define_name) {
  auto vec = stream_compact_buffer->generateCommands(abstract, define_name);

  std::string ssbo_define_scan = define_name + "_SCAN";
  std::string ssbo_define_count = define_name + "_COUNT";
  auto vec_ssbo_scan = ssbo.scan->generateCommands(abstract, ssbo_define_scan);
  auto vec_ssbo_count =
      ssbo.count->generateCommands(abstract, ssbo_define_count);
  vec.insert(vec.end(), vec_ssbo_scan.begin(), vec_ssbo_scan.end());
  vec.insert(vec.end(), vec_ssbo_count.begin(), vec_ssbo_count.end());

  return vec;
}
std::unique_ptr<BufferDataInterface>
StreamCompactBufferDataAccess::cloneBufferDataInterface() {
  return std::make_unique<StreamCompactBufferDataAccess>(
      stream_compact_buffer->clone(),
      IndexSSBOData{
          ssbo.scan->cloneBufferDataInterface(),
          ssbo.count->cloneBufferDataInterface(),
      });
}

