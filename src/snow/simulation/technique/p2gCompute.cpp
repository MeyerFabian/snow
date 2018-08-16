#include "p2gCompute.hpp"
#include <iostream>
void P2GCompute::init(string cs) {
  Technique::init();
  addShader(cs.c_str(), GL_COMPUTE_SHADER);
  Technique::upload();
}

