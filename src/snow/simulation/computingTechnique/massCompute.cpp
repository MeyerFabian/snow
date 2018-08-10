#include "massCompute.hpp"
#include <iostream>
bool MassCompute::init(std::string filename) {
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
void MassCompute::setGridPos(const float gridPosx, const float gridPosy,
                             const float gridPosz) {
  glUniform3f(gGridPos, gridPosx, gridPosy, gridPosz);
}
void MassCompute::setGridDim(const int gridDimx, const int gridDimy,
                             const int gridDimz) {
  glUniform3i(gGridDim, gridDimx, gridDimy, gridDimz);
}
void MassCompute::setGridSpacing(const float spacing) {
  glUniform1f(h, spacing);
}

void MassCompute::setYoung() { glUniform1f(young, YOUNG_MODULUS); }

void MassCompute::setPoisson() { glUniform1f(poisson, POISSON); }

void MassCompute::setHardening() { glUniform1f(hardening, HARDENING); }

void MassCompute::setCritComp() { glUniform1f(critComp, CRIT_COMPRESSION); }

void MassCompute::setCritStretch() { glUniform1f(critStretch, CRIT_STRETCH); }
void MassCompute::setIndexSize(const int size) { glUniform1i(indexSize, size); }

