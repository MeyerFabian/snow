#include "g2pCompute.hpp"
#include <iostream>
bool G2PCompute::init(std::string filename) {
  if (!addShader(std::make_shared<Shader>(ShaderType::COMPUTE, filename))) {
    return false;
  }

  for (auto& shaderObject : shaderObjects) {
    shaderObject->loadFromFile();
    shaderObject->compile();
  }

  finalize();

  gGridPos = glGetUniformLocation(this->shaderProgram, "gGridPos");
  gGridDim = glGetUniformLocation(this->shaderProgram, "gGridDim");
  h = glGetUniformLocation(this->shaderProgram, "gridSpacing");
  young = glGetUniformLocation(this->shaderProgram, "young");
  poisson = glGetUniformLocation(this->shaderProgram, "poisson");
  hardening = glGetUniformLocation(this->shaderProgram, "hardening");
  critComp = glGetUniformLocation(this->shaderProgram, "critComp");
  critStretch = glGetUniformLocation(this->shaderProgram, "critStretch");
  indexSize = glGetUniformLocation(this->shaderProgram, "indexSize");
  if (gGridPos == INVALID_UNIFORM_LOCATION ||
      gGridDim == INVALID_UNIFORM_LOCATION ||
      young == INVALID_UNIFORM_LOCATION ||
      poisson == INVALID_UNIFORM_LOCATION ||
      hardening == INVALID_UNIFORM_LOCATION ||
      critComp == INVALID_UNIFORM_LOCATION ||
      critStretch == INVALID_UNIFORM_LOCATION ||
      h == INVALID_UNIFORM_LOCATION || indexSize == INVALID_UNIFORM_LOCATION) {
    return false;
  }
  return true;
}
void G2PCompute::setGridPos(const float gridPosx, const float gridPosy,
                            const float gridPosz) {
  glUniform3f(gGridPos, gridPosx, gridPosy, gridPosz);
}
void G2PCompute::setGridDim(const int gridDimx, const int gridDimy,
                            const int gridDimz) {
  glUniform3i(gGridDim, gridDimx, gridDimy, gridDimz);
}
void G2PCompute::setGridSpacing(const float spacing) {
  glUniform1f(h, spacing);
}

void G2PCompute::setYoung() { glUniform1f(young, YOUNG_MODULUS); }

void G2PCompute::setPoisson() { glUniform1f(poisson, POISSON); }

void G2PCompute::setHardening() { glUniform1f(hardening, HARDENING); }

void G2PCompute::setCritComp() { glUniform1f(critComp, CRIT_COMPRESSION); }

void G2PCompute::setCritStretch() { glUniform1f(critStretch, CRIT_STRETCH); }
void G2PCompute::setIndexSize(const int size) { glUniform1i(indexSize, size); }

