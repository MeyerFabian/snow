#include "volumeCompute.hpp"
void VolumeCompute::init(std::string filename) {
  Technique::init();
  add_shader(std::make_shared<Shader>(ShaderType::COMPUTE, filename));
  Technique::upload();
}

