#ifndef BINNINGTECHNIQUE_H
#define BINNINGTECHNIQUE_H

#include "../../../utils/defines.hpp"

#include <glm/glm.hpp>
#include "../../../../../shader/shared_hpp/precision.hpp"
#include "../../../buffer/BufferData/IOBufferData.hpp"
#include "../../../buffer/buffer.hpp"
#include "../../../shader/technique.hpp"

class BinningTechnique : public Technique {
 public:
  LocalSize local_size = {64, 1, 1};
  bool multiple = false;
  struct UniformsStatic {};

  struct BinningData {
    std::string filename;
    bool multiple = false;
    GLuint multiple_elements = 1;
  };

  GLuint multiple_elements = 1;
  void init(BinningData&& data, IOBufferData&&);
  void uniforms_init(UniformsStatic&&);
  void dispatch_with_barrier(GLuint numVectors);
};
#endif

