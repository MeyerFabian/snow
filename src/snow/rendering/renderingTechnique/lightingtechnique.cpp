#include "lightingtechnique.hpp"

LightingTechnique::LightingTechnique() {}
bool LightingTechnique::init(string vs, string fs) {
  if (!addShader(std::make_shared<Shader>(ShaderType::VERTEX, vs))) {
    return false;
  }
  if (!addShader(std::make_shared<Shader>(ShaderType::FRAGMENT, fs))) {
    return false;
  }
  for (auto& shaderObject : shaderObjects) {
    shaderObject->loadFromFile();
    shaderObject->compile();
  }
  finalize();

  gWorldLocation = glGetUniformLocation(this->shaderProgram, "gModel");
  gSampler = glGetUniformLocation(this->shaderProgram, "gSampler");
  gLightLocation = glGetUniformLocation(this->shaderProgram, "gLightPosition");
  gAmbient = glGetUniformLocation(this->shaderProgram, "gAmbient");
  gColor = glGetUniformLocation(this->shaderProgram, "gColor");
  gDiffuse = glGetUniformLocation(this->shaderProgram, "gDiffuse");
  gMVPLocation = glGetUniformLocation(this->shaderProgram, "gMVP");
  gInverse = glGetUniformLocation(this->shaderProgram, "inverse");
  gSpecInt = glGetUniformLocation(this->shaderProgram, "gSpecInt");
  gSpecPower = glGetUniformLocation(this->shaderProgram, "gSpecPower");
  gCameraPos = glGetUniformLocation(this->shaderProgram, "gCameraPos");
  gShadowMap = glGetUniformLocation(this->shaderProgram, "gShadowMap");
  gLightMVP = glGetUniformLocation(this->shaderProgram, "gLightMVP");
  return true;
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

