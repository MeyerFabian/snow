#include "rigidCompute.hpp"
#include <iostream>
RigidCompute::RigidCompute() {}
bool RigidCompute::init(std::string filename) {
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
void RigidCompute::setDt(const float deltat) { glUniform1f(dt, deltat); }
void RigidCompute::setCritComp() { glUniform1f(critComp, CRIT_COMPRESSION); }

void RigidCompute::setCritStretch() { glUniform1f(critStretch, CRIT_STRETCH); }
void RigidCompute::setCollisionOffset() {
  glUniform1i(collisionOffset, GRID_COLLISION_PLANE_OFFSET);
}
void RigidCompute::setGridDim(const int gridDimx, const int gridDimy,
                              const int gridDimz) {
  glUniform3i(gGridDimension, gridDimx, gridDimy, gridDimz);
}
void RigidCompute::setnumColliders(const int numColliders) {
  glUniform1i(gNumColliders, numColliders);
}
void RigidCompute::setIndexSize(const int size) {
  glUniform1i(indexSize, size);
}

