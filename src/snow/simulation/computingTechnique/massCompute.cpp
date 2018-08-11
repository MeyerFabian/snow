#include "massCompute.hpp"
#include <iostream>
void MassCompute::init(std::string filename) {
  Technique::use();
  addShader(std::make_shared<Shader>(ShaderType::COMPUTE, filename));
  Technique::compile();
}

