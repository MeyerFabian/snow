#include "particleCompute.hpp"
#include <iostream>
void ParticleCompute::init(string cs) {
  Technique::init();
  addShader(cs.c_str(), GL_COMPUTE_SHADER);
  Technique::upload();
}

