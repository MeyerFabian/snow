#include "lightingtechnique.hpp"
void LightingTechnique::init(std::string vs, std::string fs) {
  addShader(std::make_shared<Shader>(ShaderType::VERTEX, vs));
  addShader(std::make_shared<Shader>(ShaderType::FRAGMENT, fs));
  Technique::compile();
}

