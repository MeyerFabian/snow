#include "volumeCompute.hpp"
#include <iostream>
void VolumeCompute::init(string cs) {
  Technique::init();
  addShader(cs.c_str(), GL_COMPUTE_SHADER);
  Technique::upload();
}

