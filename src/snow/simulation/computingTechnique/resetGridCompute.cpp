#include "resetGridCompute.hpp"
#include <iostream>
ResetGridCompute::ResetGridCompute() {}
bool ResetGridCompute::init(std::string filename) {
  if (!addShader(std::make_shared<Shader>(ShaderType::COMPUTE, filename))) {
    return false;
  }

  for (auto& shaderObject : shaderObjects) {
    shaderObject->loadFromFile();
    shaderObject->compile();
  }

  finalize();

  dt = glGetUniformLocation(this->shaderProgram, "dt");
  critComp = glGetUniformLocation(this->shaderProgram, "critComp");
  critStretch = glGetUniformLocation(this->shaderProgram, "critStretch");
  indexSize = glGetUniformLocation(this->shaderProgram, "indexSize");
  collisionOffset =
      glGetUniformLocation(this->shaderProgram, "collisionOffset");
  gGridDimension = glGetUniformLocation(this->shaderProgram, "gGridDimension");
  gNumColliders = glGetUniformLocation(this->shaderProgram, "gNumColliders");

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

