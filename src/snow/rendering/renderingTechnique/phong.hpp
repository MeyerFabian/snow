#ifndef PHONG_H
#define PHONG_H
#include "../../shader/technique.hpp"
#include "math3d.hpp"
class Phong : public Technique {
 public:
  struct UniformsStatic {
    Vector3f lightPos;
    float ambient;
    Vector3f color;
    float diffuse;
    Vector3f specIntensity;
    GLint specPower;
    Vector3f cameraPos;
  };
  struct UniformsDynamic {
    const Matrix4f* WVP;
    const Matrix4f* ModelMatrix;
  };
  void init(UniformsStatic&& uniforms);
  void uniforms_update(UniformsDynamic&& uniforms);

 private:
  void uniforms_init(UniformsStatic&& uniforms);
};

#endif  // PHONG_H

