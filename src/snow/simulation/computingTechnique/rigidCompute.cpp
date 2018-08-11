#include "rigidCompute.hpp"
void RigidCompute::init(std::string filename) {
  addShader(std::make_shared<Shader>(ShaderType::COMPUTE, filename));
  Technique::compile();
}

