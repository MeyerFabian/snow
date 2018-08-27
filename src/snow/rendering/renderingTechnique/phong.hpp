#ifndef PHONG_H
#define PHONG_H
#include <glm/glm.hpp>
#include "../../shader/technique.hpp"
class Phong : public Technique {
 public:
  struct UniformsStatic {
    glm::vec3 lightPos;
    float ambient;
    glm::vec3 color;
    float diffuse;
    glm::vec3 specIntensity;
    GLint specPower;
    glm::vec3 cameraPos;
  };
  struct UniformsDynamic {
    const glm::mat4& WVP;
    const glm::mat4& ModelMatrix;
  };
  void init(UniformsStatic&& uniforms);
  void uniforms_update(UniformsDynamic&& uniforms);

 private:
  void uniforms_init(UniformsStatic&& uniforms);
};

#endif  // PHONG_H

