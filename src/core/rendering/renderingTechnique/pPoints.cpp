#include "pPoints.hpp"

void PPoints::init(UniformsStatic&& uniforms) {
  Technique::add_shader(std::make_shared<Shader>(ShaderType::VERTEX,
                                                 "shader/render/pPoints.vert"));
  Technique::add_shader(std::make_shared<Shader>(ShaderType::FRAGMENT,
                                                 "shader/render/pPoints.frag"));
  Technique::upload();
  Technique::use();
  uniforms_init(std::move(uniforms));
}
void PPoints::uniforms_init(UniformsStatic&& uniforms) {}
void PPoints::uniforms_update(UniformsDynamic&& uniforms) {
  Technique::use();
  uniform_update("gMVP", uniforms.WVP);
}

