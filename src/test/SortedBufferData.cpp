#include "SortedBufferData.hpp"
void SortedBufferData::setName(std::string name) {
  buffer_interface->setName(name);
}
std::string SortedBufferData::getName() { return buffer_interface->getName(); }

void SortedBufferData::setIndexBuffer(std::string in_index) {
  buffer_interface->setIndexBuffer(in_index);
}
std::string SortedBufferData::getIndexBuffer() {
  return buffer_interface->getIndexBuffer();
}

void SortedBufferData::setVariable(std::string name) {
  buffer_interface->setVariable(name);
}
std::string SortedBufferData::getVariable() {
  return buffer_interface->getVariable();
}

GLuint SortedBufferData::getSize() { return buffer_interface->getSize(); }

std::unique_ptr<BufferDataInterface>
SortedBufferData::cloneBufferDataInterface() const {
  return std::make_unique<SortedBufferData>(
      sorting_method, buffer_interface->cloneBufferDataInterface());
};

std::vector<Shader::CommandType> SortedBufferData::generateCommands(
    bool abstract, std::string define_name) {
  auto vec = buffer_interface->generateCommands(abstract, define_name);

  if (sorting_method == SortingMethod::Index) {
    vec.insert(vec.end(), {PreprocessorCmd::DEFINE,
                           define_name + "_SORTING_METHOD index"});
  }
  return vec;
}

