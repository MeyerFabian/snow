#include "p2gCompute.hpp"
void P2GCompute::init(std::string filename) {
  Technique::use();
  addShader(std::make_shared<Shader>(ShaderType::COMPUTE, filename));
  Technique::compile();
}

