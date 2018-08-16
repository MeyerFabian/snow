#include "g2pCompute.hpp"
void G2PCompute::init(std::string filename) {
  Technique::init();
  add_shader(ShaderType::COMPUTE, filename);
  Technique::upload();
}

