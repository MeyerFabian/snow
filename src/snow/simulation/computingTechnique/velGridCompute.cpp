#include "velGridCompute.hpp"
void VelGridCompute::init(std::string filename) {
  addShader(std::make_shared<Shader>(ShaderType::COMPUTE, filename));
  Technique::compile();
}

