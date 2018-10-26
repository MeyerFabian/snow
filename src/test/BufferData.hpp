#ifndef BUFFERDATA_HPP_IUIPQ6XV
#define BUFFERDATA_HPP_IUIPQ6XV
#include "../snow/buffer/buffer.hpp"
struct BufferData {
  std::string name;
  std::string var;
  BufferInfo info;
  GLuint bufferSize;
  GLuint bufferIndex = 0;
  GLuint bufferNum = 1;
  std::string var_size_def = "1";
};

#endif /* end of include guard: BUFFERDATA_HPP_IUIPQ6XV */

