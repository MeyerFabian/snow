#ifndef GFORCECOMPUTE_H
#define GFORCECOMPUTE_H
#include "../../shader/technique.hpp"
#include "../../utils/defines.hpp"
class GForceCompute : public Technique {
 public:
  struct UniformsStatic {
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
};

#endif  // GFORCECOMPUTE_H

