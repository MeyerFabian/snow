#ifndef P2GATOMICCOMPUTE_H
#define P2GATOMICCOMPUTE_H
#include <memory>

#include "../../snow/utils/defines.hpp"

#include <glm/glm.hpp>
#include "../../../shader/shared_hpp/precision.hpp"
#include "../../snow/shader/technique.hpp"
#include "../../snow/utils/defines.hpp"
#include "../IOBufferData.hpp"
class P2G_atomic_global : public Technique {
 public:
  LocalSize local_size = {1024, 1, 1};
  struct UniformsStatic {
    PREC_VEC3_TYPE gGridPos;
    glm::uvec3 gGridDim;
    PREC_SCAL_TYPE gridSpacing;
  };
  struct P2GData {
    UniformsStatic uniforms;
  };

  struct UniformsDynamic {
    GLuint numParticles;
  };
  void dispatch(UniformsDynamic&& uniforms);
  void dispatch_with_barrier(UniformsDynamic&& uniforms);
  void init_too_parallel(P2GData&& data, IOBufferData&& io);
  void init_looping(P2GData&& data, IOBufferData&& io);

 private:
  void init(P2GData&& data, IOBufferData&& io);
  void uniforms_init(UniformsStatic&& uniforms);
  void uniforms_update(UniformsDynamic&& uniforms);
  std::string filename;
  GLuint ydim;
};
#endif  // P2GCOMPUTE_H

