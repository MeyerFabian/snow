#include "resetGridCompute.hpp"
#include <iostream>
ResetGridCompute::ResetGridCompute() {}
bool ResetGridCompute::init(string cs) {
  Technique::init();
  addShader(cs.c_str(), GL_COMPUTE_SHADER);

  finalize();

  dt = glGetUniformLocation(this->ShaderProgram, "dt");
  critComp = glGetUniformLocation(this->ShaderProgram, "critComp");
  critStretch = glGetUniformLocation(this->ShaderProgram, "critStretch");
  indexSize = glGetUniformLocation(this->ShaderProgram, "indexSize");
  collisionOffset =
      glGetUniformLocation(this->ShaderProgram, "collisionOffset");
  gGridDimension = glGetUniformLocation(this->ShaderProgram, "gGridDimension");
  gNumColliders = glGetUniformLocation(this->ShaderProgram, "gNumColliders");

  return true;
}
void ResetGridCompute::setDt(const float deltat) { glUniform1f(dt, deltat); }
void ResetGridCompute::setCritComp() {
  glUniform1f(critComp, CRIT_COMPRESSION);
}

void ResetGridCompute::setCritStretch() {
  glUniform1f(critStretch, CRIT_STRETCH);
}
void ResetGridCompute::setCollisionOffset() {
  glUniform1i(collisionOffset, GRID_COLLISION_PLANE_OFFSET);
}
void ResetGridCompute::setGridDim(const int gridDimx, const int gridDimy,
                                  const int gridDimz) {
  glUniform3i(gGridDimension, gridDimx, gridDimy, gridDimz);
}
void ResetGridCompute::setnumColliders(const int numColliders) {
  glUniform1i(gNumColliders, numColliders);
}
void ResetGridCompute::setIndexSize(const int size) {
  glUniform1i(indexSize, size);
}

