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
  LocalSize local_size = {32, 1, 1};

  struct UniformsStatic {
    PREC_VEC3_TYPE gGridPos;
    glm::uvec3 gGridDim;
    PREC_SCAL_TYPE gridSpacing;
  };

  struct BinningData {
    UniformsStatic uniforms;
  };

  void init(BinningData&& data, IOBufferDataInterface&&);
  void uniforms_init(UniformsStatic&&);
  void dispatch_with_barrier(GLuint numVectors);
};
#endif

