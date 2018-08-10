#include "lightingtechnique.hpp"

LightingTechnique::LightingTechnique() {}
void LightingTechnique::init(string vs, string fs) {
  addShader(vs.c_str(), GL_VERTEX_SHADER);
  addShader(fs.c_str(), GL_FRAGMENT_SHADER);

  finalize();

  gWorldLocation = glGetUniformLocation(this->ShaderProgram, "gModel");
  gSampler = glGetUniformLocation(this->ShaderProgram, "gSampler");
  gLightLocation = glGetUniformLocation(this->ShaderProgram, "gLightPosition");
  gAmbient = glGetUniformLocation(this->ShaderProgram, "gAmbient");
  gColor = glGetUniformLocation(this->ShaderProgram, "gColor");
  gDiffuse = glGetUniformLocation(this->ShaderProgram, "gDiffuse");
  gMVPLocation = glGetUniformLocation(this->ShaderProgram, "gMVP");
  gInverse = glGetUniformLocation(this->ShaderProgram, "inverse");
  gSpecInt = glGetUniformLocation(this->ShaderProgram, "gSpecInt");
  gSpecPower = glGetUniformLocation(this->ShaderProgram, "gSpecPower");
  gCameraPos = glGetUniformLocation(this->ShaderProgram, "gCameraPos");
  gShadowMap = glGetUniformLocation(this->ShaderProgram, "gShadowMap");
  gLightMVP = glGetUniformLocation(this->ShaderProgram, "gLightMVP");
}

void LightingTechnique::setShadowMapTexture(unsigned int texture) {
  glUniform1i(gShadowMap, texture);
}

void LightingTechnique::setLightMVP(const Matrix4f* LightMVP) {
  glUniformMatrix4fv(gLightMVP, 1, GL_TRUE, (const GLfloat*)LightMVP);
}

void LightingTechnique::setCameraPos(Vector3f camerapos) {
  glUniform3f(gCameraPos, camerapos.x, camerapos.y, camerapos.z);
}

void LightingTechnique::setSpecularPower(int power) {
  glUniform1i(gSpecPower, power);
}

void LightingTechnique::setSpecularIntensity(Vector3f intensity) {
  glUniform3f(gSpecInt, intensity.x, intensity.y, intensity.z);
}

void LightingTechnique::setWVP(const Matrix4f* MVP) {
  glUniformMatrix4fv(gMVPLocation, 1, GL_TRUE, (const GLfloat*)MVP);
}

void LightingTechnique::setWorldMatrix(const Matrix4f* WorldMatrix) {
  glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const GLfloat*)WorldMatrix);
}

void LightingTechnique::setInverse(const glm::mat4* inverse) {
  glUniformMatrix4fv(gInverse, 1, GL_TRUE, (const GLfloat*)inverse);
}

void LightingTechnique::setSampler(int sampler) {
  glUniform1i(gSampler, sampler);
}
void LightingTechnique::setLight(Vector3f LightPosition, float ambient,
                                 Vector3f Color, float diffuse) {
  glUniform3f(gLightLocation, LightPosition.x, LightPosition.y,
              LightPosition.z);
  glUniform1f(gAmbient, ambient);
  glUniform3f(gColor, Color.x, Color.y, Color.z);
  glUniform1f(gDiffuse, diffuse);
}

