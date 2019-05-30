#ifndef RIGIDCOMPUTE_H
#define RIGIDCOMPUTE_H
#include "../../shader/technique.hpp"
#include "../../utils/defines.hpp"
class RigidAdvance : public Technique {
 public:
  struct UniformsStatic {};

  struct UniformsDynamic {
    GLuint numColliders;
    double dt;
  };
  void init(UniformsStatic&& uniforms);
  void dispatch(UniformsDynamic&& uniforms);
  void dispatch_with_barrier(UniformsDynamic&& uniforms);

 private:
  void uniforms_init(UniformsStatic&& uniforms);
  void uniforms_update(UniformsDynamic&& uniforms);
};

#endif  // RIGIDCOMPUTE_H

