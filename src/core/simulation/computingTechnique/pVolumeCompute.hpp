#ifndef PVOLUMECOMPUTE_H
#define PVOLUMECOMPUTE_H
#include "../../shader/technique.hpp"
#include "../../utils/defines.hpp"
class PVolumeCompute : public Technique {
 public:
  struct UniformsStatic {
    GLuint numParticles;
  };

  void init(UniformsStatic&& uniforms);
  void dispatch();
  void dispatch_with_barrier();

 private:
  void uniforms_init(UniformsStatic&& uniforms);
  GLuint m_numParticles;
};
#endif  // PVOLUMECOMPUTE_H

