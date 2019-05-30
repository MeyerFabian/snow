#ifndef G2PCOMPUTE_H
#define G2PCOMPUTE_H
#include "../../shader/technique.hpp"
#include "../../utils/defines.hpp"
class G2PTransfer : public Technique {
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
#endif  // G2PCOMPUTE_H

