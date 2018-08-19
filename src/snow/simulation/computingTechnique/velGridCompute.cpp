#include "velGridCompute.hpp"
void VelGridCompute::init(std::string filename) {
  add_shader(std::make_shared<Shader>(ShaderType::COMPUTE, filename));
  Technique::upload();
}
