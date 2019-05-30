#ifndef PPOINTS_H
#define PPOINTS_H
#include <glm/glm.hpp>
#include "../../shader/technique.hpp"
class PPoints : public Technique {
 public:
  struct UniformsStatic {};
  struct UniformsDynamic {
    const glm::mat4& WVP;
  };
  void init(UniformsStatic&& uniforms);
  void uniforms_update(UniformsDynamic&& uniforms);

 private:
  void uniforms_init(UniformsStatic&& uniforms);
};

#endif  // PPOINTS_H

