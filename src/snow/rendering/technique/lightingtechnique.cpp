#include "lightingtechnique.hpp"
void LightingTechnique::init(std::string vs, std::string fs) {
  Technique::init();
  add_shader(std::make_shared<Shader>(ShaderType::VERTEX, vs));
  add_shader(std::make_shared<Shader>(ShaderType::FRAGMENT, fs));
  Technique::upload();
}

