#include "phong.hpp"

void Phong::init(UniformsStatic&& uniforms) {
  Technique::add_shader(
      std::make_shared<Shader>(ShaderType::VERTEX, "shader/render/phong.vert"));
  Technique::add_shader(std::make_shared<Shader>(ShaderType::FRAGMENT,
                                                 "shader/render/phong.frag"));
  Technique::upload();
  Technique::use();
  uniforms_init(std::move(uniforms));
}
void Phong::uniforms_init(UniformsStatic&& uniforms) {
  uniform_update("gLightPosition", uniforms.lightPos.x, uniforms.lightPos.y,
                 uniforms.lightPos.z);
  uniform_update("gAmbient", uniforms.ambient);
  uniform_update("gColor", uniforms.color.x, uniforms.color.y,
                 uniforms.color.z);
  uniform_update("gDiffuse", uniforms.diffuse);
  uniform_update("gSpecInt", uniforms.specIntensity.x, uniforms.specIntensity.y,
                 uniforms.specIntensity.z);
  uniform_update("gSpecPower", uniforms.specPower);
  uniform_update("gCameraPos", uniforms.cameraPos.x, uniforms.cameraPos.y,
                 uniforms.cameraPos.z);
}
void Phong::uniforms_update(UniformsDynamic&& uniforms) {
  Technique::use();
  uniform_update("gMVP", uniforms.WVP);
  uniform_update("gModel", uniforms.ModelMatrix);
}

