#include "volumeCompute.hpp"
void VolumeCompute::init(std::string filename) {
  Technique::use();
  addShader(std::make_shared<Shader>(ShaderType::COMPUTE, filename));
  Technique::compile();
}

