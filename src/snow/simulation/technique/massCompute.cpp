#include "massCompute.hpp"
#include <iostream>
void MassCompute::init(string cs) {
  Technique::init();
  addShader(cs.c_str(), GL_COMPUTE_SHADER);
  Technique::upload();
}

