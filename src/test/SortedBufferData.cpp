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

