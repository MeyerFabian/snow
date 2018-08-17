#ifndef TECHNIQUE_H
#define TECHNIQUE_H

#define GLEW_STATIC
#include <GL/glew.h>

#include <string.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "math3d.hpp"
#include "shader.hpp"
using namespace std;

class Technique {
 protected:
  GLuint shaderProgram;

 private:
  std::vector<std::shared_ptr<Shader>> shaderObjects;
  std::unordered_map<std::string, GLuint> m_uniformMap;
  void gl_uniforms_read();
  void attach_and_link() const;
  void attach() const;
  void gl_link() const;
  void gl_attach(GLuint) const;

 public:
  void uniform_update(const std::string&, float) const noexcept;
  void uniform_update(const std::string&, GLint) const noexcept;
  void uniform_update(const std::string&, GLuint) const noexcept;
  void uniform_update(const std::string&, double) const noexcept;
  void uniform_update(const std::string&, float, float, float) const noexcept;
  void uniform_update(const std::string&, const Matrix4f*) const noexcept;
  void uniform_update(const std::string&, GLint, GLint, GLint) const noexcept;
  GLuint uniform_look_up(std::string) const noexcept;
  void init();
  void upload();
  ~Technique();
  GLuint getShaderProgram();
  void plugTechnique();
  bool add_shader(std::shared_ptr<Shader>&& ptr);
};

#endif  // TECHNIQUE_H

