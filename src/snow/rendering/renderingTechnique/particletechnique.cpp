#include "particletechnique.hpp"
#include <iostream>
ParticleTechnique::ParticleTechnique() {}
void ParticleTechnique::setWVP(const Matrix4f* MVP) {
  glUniformMatrix4fv(ModelViewPersp, 1, GL_TRUE, (const GLfloat*)MVP);
}
bool ParticleTechnique::init(std::string vs, std::string fs) {
  addShader(std::make_shared<Shader>(ShaderType::VERTEX, vs));
  addShader(std::make_shared<Shader>(ShaderType::FRAGMENT, fs));
  for (auto& shaderObject : shaderObjects) {
    shaderObject->loadFromFile();
    shaderObject->compile();
  }
  finalize();

  ModelViewPersp = glGetUniformLocation(this->shaderProgram, "gMVP");

  if (ModelViewPersp == INVALID_UNIFORM_LOCATION) {
    return false;
  }
  return true;
}

