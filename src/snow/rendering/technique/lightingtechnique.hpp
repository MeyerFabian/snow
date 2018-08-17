#ifndef LIGHTINGTECHNIQUE_H
#define LIGHTINGTECHNIQUE_H
#include <string>
#include "../../shader/technique.hpp"
#include "glm.hpp"
#include "math3d.hpp"
using namespace std;
class LightingTechnique : public Technique {
 public:
  void init(string vs, string fs);
  void setWVP(const Matrix4f*);
  void setWorldMatrix(const Matrix4f*);
  void setSampler(int sampler);
  void setLight(Vector3f light, float ambient, Vector3f diffuseColor,
                float diffuse);
  void setInverse(const glm::mat4* inverse);
  void setCameraPos(Vector3f camerapos);
  void setSpecularPower(int power);
  void setSpecularIntensity(Vector3f intensity);
  void setShadowMapTexture(unsigned int texture);
  void setLightMVP(const Matrix4f* MVP);

 private:
  GLuint gWorldLocation;
  GLuint gSampler;
  GLuint gShadowMap;
  GLuint gLightMVP;
  GLuint gLightLocation;
  GLuint gAmbient;
  GLuint gDiffuse;
  GLuint gColor;
  GLuint gMVPLocation;
  GLuint gInverse;
  GLuint gSpecInt;
  GLuint gSpecPower;
  GLuint gCameraPos;
};

#endif  // LIGHTINGTECHNIQUE_H

