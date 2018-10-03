#ifndef COUNTINGTECHNIQUE_H
#define COUNTINGTECHNIQUE_H
#include <glm/glm.hpp>

#include "../../../shader/shared_hpp/precision.hpp"
#include "../../snow/buffer/buffer.hpp"
#include "../../snow/shader/technique.hpp"
#include "../IOBufferData.hpp"

class CountingTechnique : public Technique {
 public:
  LocalSize local_size = {32, 1, 1};
  struct UniformsStatic {
    PREC_VEC3_TYPE gGridPos;
    glm::uvec3 gGridDim;
    PREC_SCAL_TYPE gridSpacing;
  };

  struct CountingData {
    IOBufferData io;
    UniformsStatic uniforms;
    LocalSize local_size;
    GLuint numVectors;  // SOA only
  };

  void init(CountingData&& data);
  void uniforms_init(UniformsStatic&&);
  void dispatch_with_barrier(GLuint numVectors);
};
#endif

