#include "resetGridCompute.hpp"
void ResetGridCompute::init(std::string filename) {
  Technique::use();
  addShader(std::make_shared<Shader>(ShaderType::COMPUTE, filename));
  Technique::compile();
}

