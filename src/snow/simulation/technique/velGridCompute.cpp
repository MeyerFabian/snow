#include "velGridCompute.hpp"
void VelGridCompute::init(std::string filename) {
  Technique::init();
  add_shader(ShaderType::COMPUTE, filename);
  Technique::upload();
}

