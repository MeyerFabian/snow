#include "p2gCompute.hpp"
void P2GCompute::init(std::string filename) {
  add_shader(std::make_shared<Shader>(ShaderType::COMPUTE, filename));
  Technique::upload();
}

