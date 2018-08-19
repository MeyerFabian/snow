#include "particletechnique.hpp"
void ParticleTechnique::init(std::string vs, std::string fs) {
  add_shader(std::make_shared<Shader>(ShaderType::VERTEX, vs));
  add_shader(std::make_shared<Shader>(ShaderType::FRAGMENT, fs));
  Technique::upload();
}
