#include "rigidCompute.hpp"
void RigidCompute::init(std::string filename) {
  Technique::init();
  add_shader(ShaderType::COMPUTE, filename);
  Technique::upload();
}

