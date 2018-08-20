#ifndef P2GCOMPUTE_H
#define P2GCOMPUTE_H
#include <memory>
#include "../../shader/technique.hpp"
#include "../../utils/defines.hpp"
class P2GTransfer : public Technique {
 public:
  struct UniformsStatic {
    GLuint numParticles;
  };

  struct UniformsDynamic {};
  void init(UniformsStatic&& uniforms);
  void dispatch(UniformsDynamic&& uniforms);
  void dispatch_with_barrier(UniformsDynamic&& uniforms);

 private:
  void uniforms_init(UniformsStatic&& uniforms);
  void uniforms_update(UniformsDynamic&& uniforms);
  GLuint m_numParticles;
};
#endif  // P2GCOMPUTE_H

