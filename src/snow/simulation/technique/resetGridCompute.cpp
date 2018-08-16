#include "resetGridCompute.hpp"
void ResetGridCompute::init(std::string filename) {
  Technique::init();
  add_shader(ShaderType::COMPUTE, filename);
  Technique::upload();
}

