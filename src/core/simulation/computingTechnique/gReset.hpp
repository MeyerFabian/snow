#ifndef RESETGRID_H
#define RESETGRID_H
#include "../../shader/technique.hpp"
#include "../../utils/defines.hpp"
class GReset : public Technique {
 public:
  struct UniformsInit {};

  struct UniformsUpdate {};
  void init(UniformsInit&& uniforms);
  void dispatch(UniformsUpdate&& uniforms);
  void dispatch_with_barrier(UniformsUpdate&& uniforms);

 private:
  void uniforms_init(UniformsInit&& uniforms);
  void uniforms_update(UniformsUpdate&& uniforms);
};

#endif  // RESETGRID_H

