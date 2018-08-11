#include "particletechnique.hpp"
void ParticleTechnique::init(std::string vs, std::string fs) {
  addShader(std::make_shared<Shader>(ShaderType::VERTEX, vs));
  addShader(std::make_shared<Shader>(ShaderType::FRAGMENT, fs));
  Technique::compile();
}

