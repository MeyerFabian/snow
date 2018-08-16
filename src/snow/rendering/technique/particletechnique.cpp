#include "particletechnique.hpp"
void ParticleTechnique::init(std::string vs, std::string fs) {
  Technique::init();
  add_shader(ShaderType::VERTEX, vs);
  add_shader(ShaderType::FRAGMENT, fs);
  Technique::upload();
}

