#ifndef MASSCOMPUTE_H
#define MASSCOMPUTE_H
#include "../../shader/technique.hpp"
#include "../../utils/defines.hpp"
class GMassCompute : public Technique {
 public:
 public:
  struct UniformsStatic {
    GLuint numParticles;
  };

  void init(UniformsStatic&& uniforms);
  void dispatch();
  void dispatch_with_barrier();

 private:
  void uniforms_init(UniformsStatic&& uniforms);
  void uniforms_update();
  GLuint m_numParticles;
};
#endif  // MASSCOMPUTE_H

