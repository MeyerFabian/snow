#include "resetGridCompute.hpp"
#include <iostream>
void ResetGridCompute::init(string cs) {
  Technique::init();
  addShader(cs.c_str(), GL_COMPUTE_SHADER);
  Technique::upload();
}

