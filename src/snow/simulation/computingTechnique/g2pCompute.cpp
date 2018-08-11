#include "g2pCompute.hpp"
void G2PCompute::init(std::string filename) {
  addShader(std::make_shared<Shader>(ShaderType::COMPUTE, filename));
  Technique::compile();
}

