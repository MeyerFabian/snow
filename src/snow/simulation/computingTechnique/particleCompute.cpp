#include "particleCompute.hpp"
#include <iostream>
ParticleCompute::ParticleCompute() {}
bool ParticleCompute::init(std::string filename) {
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
void ParticleCompute::setDt(const float deltat) { glUniform1f(dt, deltat); }
void ParticleCompute::setCritComp() { glUniform1f(critComp, CRIT_COMPRESSION); }

void ParticleCompute::setCritStretch() {
  glUniform1f(critStretch, CRIT_STRETCH);
}
void ParticleCompute::setCollisionOffset() {
  glUniform1i(collisionOffset, GRID_COLLISION_PLANE_OFFSET);
}
void ParticleCompute::setGridDim(const int gridDimx, const int gridDimy,
                                 const int gridDimz) {
  glUniform3i(gGridDimension, gridDimx, gridDimy, gridDimz);
}
void ParticleCompute::setnumColliders(const int numColliders) {
  glUniform1i(gNumColliders, numColliders);
}
void ParticleCompute::setIndexSize(const int size) {
  glUniform1i(indexSize, size);
}

