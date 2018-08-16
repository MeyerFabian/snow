#include "shadowMapTechnique.hpp"
void ShadowMapTechnique::init(std::string vs, std::string fs) {
  Technique::init();
  add_shader(ShaderType::VERTEX, vs);
  add_shader(ShaderType::FRAGMENT, fs);
  // update uniform mvp, gSampler
  Technique::upload();
}

