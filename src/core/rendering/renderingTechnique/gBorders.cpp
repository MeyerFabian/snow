#include "gBorders.hpp"

void GBorders::init(UniformsStatic&& uniforms) {
  Technique::add_shader(std::make_shared<Shader>(
      ShaderType::VERTEX, "shader/render/gBorders.vert"));
  Technique::add_shader(std::make_shared<Shader>(
      ShaderType::FRAGMENT, "shader/render/gBorders.frag"));
  Technique::upload();
  Technique::use();
  uniforms_init(std::move(uniforms));
}
void GBorders::uniforms_init(UniformsStatic&& uniforms) {}
void GBorders::uniforms_update(UniformsDynamic&& uniforms) {
  Technique::use();
  uniform_update("gMVP", uniforms.WVP);
}

