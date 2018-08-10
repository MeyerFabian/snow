#ifndef SHADOW_MAP_TECHNIQUE_H
#define SHADOW_MAP_TECHNIQUE_H
#include "../../shader/technique.hpp"
#include "math3d.hpp"
class ShadowMapTechnique : public Technique {
 public:
  ShadowMapTechnique();
  bool init(std::string vs, std::string fs);
  void setMVP(const Matrix4f* m);
  void setTex(unsigned int texture);

 private:
  GLuint gMVP;
  GLuint gSampler;
};

#endif  // SHADOW_MAP_TECHNIQUE_H

