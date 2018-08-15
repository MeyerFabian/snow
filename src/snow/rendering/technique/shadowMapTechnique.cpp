#include "shadowMapTechnique.hpp"
void ShadowMapTechnique::init(std::string vs, std::string fs) {
  Technique::init();
  add_shader(std::make_shared<Shader>(ShaderType::VERTEX, vs));
  add_shader(std::make_shared<Shader>(ShaderType::FRAGMENT, fs));
  // update uniform mvp, gSampler
  Technique::upload();
}

