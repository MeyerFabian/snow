#include "shadowMapTechnique.hpp"

ShadowMapTechnique::ShadowMapTechnique() {}
bool ShadowMapTechnique::init(std::string vs, std::string fs) {
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

  gMVP = glGetUniformLocation(this->shaderProgram, "gMVP");
  gSampler = glGetUniformLocation(this->shaderProgram, "gSampler");

  if (gMVP == INVALID_UNIFORM_LOCATION ||
      gSampler == INVALID_UNIFORM_LOCATION) {
    return false;
  }
  return true;
}

void ShadowMapTechnique::setMVP(const Matrix4f* m) {
  glUniformMatrix4fv(gMVP, 1, GL_TRUE, (const GLfloat*)m);
}
void ShadowMapTechnique::setTex(unsigned int texture) {
  glUniform1i(gSampler, texture);
}

