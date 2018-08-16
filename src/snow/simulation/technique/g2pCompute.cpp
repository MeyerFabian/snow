#include "g2pCompute.hpp"
#include <iostream>
void G2PCompute::init(string cs) {
  Technique::init();
  addShader(cs.c_str(), GL_COMPUTE_SHADER);
  Technique::upload();
}

