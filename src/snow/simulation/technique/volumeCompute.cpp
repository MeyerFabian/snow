#include "volumeCompute.hpp"
#include <iostream>
bool VolumeCompute::init(string cs) {
  Technique::init();
  addShader(cs.c_str(), GL_COMPUTE_SHADER);

  finalize();

  gGridPos = glGetUniformLocation(this->ShaderProgram, "gGridPos");
  gGridDim = glGetUniformLocation(this->ShaderProgram, "gGridDim");
  h = glGetUniformLocation(this->ShaderProgram, "gridSpacing");
  young = glGetUniformLocation(this->ShaderProgram, "young");
  poisson = glGetUniformLocation(this->ShaderProgram, "poisson");
  hardening = glGetUniformLocation(this->ShaderProgram, "hardening");
  critComp = glGetUniformLocation(this->ShaderProgram, "critComp");
  critStretch = glGetUniformLocation(this->ShaderProgram, "critStretch");
  indexSize = glGetUniformLocation(this->ShaderProgram, "indexSize");
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
void VolumeCompute::setGridPos(const float gridPosx, const float gridPosy,
                               const float gridPosz) {
  glUniform3f(gGridPos, gridPosx, gridPosy, gridPosz);
}
void VolumeCompute::setGridDim(const int gridDimx, const int gridDimy,
                               const int gridDimz) {
  glUniform3i(gGridDim, gridDimx, gridDimy, gridDimz);
}
void VolumeCompute::setGridSpacing(const float spacing) {
  glUniform1f(h, spacing);
}

void VolumeCompute::setYoung() { glUniform1f(young, YOUNG_MODULUS); }

void VolumeCompute::setPoisson() { glUniform1f(poisson, POISSON); }

void VolumeCompute::setHardening() { glUniform1f(hardening, HARDENING); }

void VolumeCompute::setCritComp() { glUniform1f(critComp, CRIT_COMPRESSION); }

void VolumeCompute::setCritStretch() { glUniform1f(critStretch, CRIT_STRETCH); }
void VolumeCompute::setIndexSize(const int size) {
  glUniform1i(indexSize, size);
}

