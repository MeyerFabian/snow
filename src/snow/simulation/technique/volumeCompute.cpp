#include "volumeCompute.hpp"
void VolumeCompute::init(std::string filename) {
  Technique::init();
  add_shader(ShaderType::COMPUTE, filename);
  Technique::upload();
}

