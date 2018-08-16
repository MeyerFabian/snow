#include "rigidCompute.hpp"
#include <iostream>
void RigidCompute::init(string cs) {
  Technique::init();
  addShader(cs.c_str(), GL_COMPUTE_SHADER);
  Technique::upload();
}

