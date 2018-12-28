#include "StreamCompactBufferData.hpp"
void StreamCompactBufferData::setName(std::string name) {
  buffer_interface->setName(name);
}
std::string StreamCompactBufferData::getName() {
  return buffer_interface->getName();
}

void StreamCompactBufferData::setIndexBuffer(std::string in_index) {
  buffer_interface->setIndexBuffer(in_index);
}
std::string StreamCompactBufferData::getIndexBuffer() {
  return buffer_interface->getIndexBuffer();
}

void StreamCompactBufferData::setVariable(std::string name) {
  buffer_interface->setVariable(name);
}
std::string StreamCompactBufferData::getVariable() {
  return buffer_interface->getVariable();
}

GLuint StreamCompactBufferData::getSize() {
  return buffer_interface->getSize();
}

