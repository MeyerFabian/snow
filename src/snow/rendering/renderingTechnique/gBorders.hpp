#ifndef GBORDERS_H
#define GBORDERS_H
#include "../../shader/technique.hpp"
#include "math3d.hpp"
class GBorders : public Technique {
 public:
  struct UniformsStatic {};
  struct UniformsDynamic {
    const Matrix4f* WVP;
  };
  void init(UniformsStatic&& uniforms);
  void uniforms_update(UniformsDynamic&& uniforms);

 private:
  void uniforms_init(UniformsStatic&& uniforms);
};

#endif  // GBORDERS_H

