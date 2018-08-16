#include "particletechnique.hpp"
#include <iostream>
void ParticleTechnique::init(string vs, string fs) {
  Technique::init();
  addShader(vs.c_str(), GL_VERTEX_SHADER);
  addShader(fs.c_str(), GL_FRAGMENT_SHADER);
  Technique::upload();
}

