#ifndef BINNINGTECHNIQUE_H
#define BINNINGTECHNIQUE_H

#include "../../snow/utils/defines.hpp"

#include <glm/glm.hpp>
#include "../../../shader/shared_hpp/precision.hpp"
#include "../../snow/buffer/buffer.hpp"
#include "../../snow/shader/technique.hpp"
#include "../IOBufferData.hpp"
class BinningTechnique : public Technique {
 public:
  LocalSize local_size = {64, 1, 1};
  bool multiple = false;
  struct UniformsStatic {
    PREC_VEC3_TYPE gGridPos;
    glm::uvec3 gGridDim;
    PREC_SCAL_TYPE gridSpacing;
  };

  struct BinningData {
    std::string filename;
    UniformsStatic uniforms;
    bool multiple = false;
    GLuint multiple_elements = 1;
  };

  GLuint multiple_elements = 1;
  void init(BinningData&& data, IOBufferData&&);
  void uniforms_init(UniformsStatic&&);
  void dispatch_with_barrier(GLuint numVectors);
};
#endif

