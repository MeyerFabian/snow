#ifndef P2GCOMPUTE_H
#define P2GCOMPUTE_H
#include <memory>
#include "../../shader/technique.hpp"
#include "../../utils/defines.hpp"
class P2GCompute : public Technique {
 public:
  struct UniformsInit {
    GLuint numParticles;
  };

  struct UniformsUpdate {};
  void init(UniformsInit&& uniforms);
  void dispatch(UniformsUpdate&& uniforms);

 private:
  void uniforms_init(UniformsInit&& uniforms);
  void uniforms_update(UniformsUpdate&& uniforms);
  GLuint numParticles;
};
#endif  // P2GCOMPUTE_H

