#include "g2pCompute.hpp"
void G2PCompute::init(std::string filename) {
  add_shader(std::make_shared<Shader>(ShaderType::COMPUTE, filename));
  Technique::upload();
}

