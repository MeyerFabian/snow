#ifndef PARTICLEADVANCEE_H
#define PARTICLEADVANCE_H
#include "../../shader/technique.hpp"
#include "../../utils/defines.hpp"
class ParticleAdvance : public Technique {
 public:
  struct UniformsStatic {
    GLuint numParticles;
    GLuint numColliders;
  };

  struct UniformsDynamic {
    double dt;
  };
  void init(UniformsStatic&& uniforms);
  void dispatch(UniformsDynamic&& uniforms);
  void dispatch_with_barrier(UniformsDynamic&& uniforms);

 private:
  void uniforms_init(UniformsStatic&& uniforms);
  void uniforms_update(UniformsDynamic&& uniforms);
  GLuint m_numParticles;
};
#endif  // PARTICLEADVANCE_H

