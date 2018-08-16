#include "velGridCompute.hpp"
#include <iostream>
void VelGridCompute::init(string cs) {
  Technique::init();
  addShader(cs.c_str(), GL_COMPUTE_SHADER);
  Technique::upload();
}

