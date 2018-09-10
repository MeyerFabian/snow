#ifndef TECHNIQUE_H
#define TECHNIQUE_H

#define GLEW_STATIC
#include <GL/glew.h>

#include <algorithm>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "shader.hpp"

class Technique {
 protected:
  Technique();
  ~Technique();
  bool add_shader(std::shared_ptr<Shader>&& ptr);

 public:
  void uniform_update(const std::string&, float) const noexcept;
  void uniform_update(const std::string&, GLint) const noexcept;
  void uniform_update(const std::string&, GLuint) const noexcept;
  void uniform_update(const std::string&, double) const noexcept;
  void uniform_update(const std::string&, float, float, float) const noexcept;
  void uniform_update(const std::string&, const glm::mat4&) const noexcept;
  void uniform_update(const std::string&, GLint, GLint, GLint) const noexcept;
  void uniform_update(const std::string& name, double x, double y,
                      double z) const noexcept;
  void use() const;
  void upload();

 private:
  GLuint uniform_look_up(std::string) const noexcept;

  void attach_and_link() const;
  void attach() const;
  void gl_link() const;
  void gl_attach(GLuint) const;

  void gl_uniforms_read();

  GLuint shaderProgram;
  bool original;
  std::vector<std::shared_ptr<Shader>> shaderObjects;
  std::unordered_map<std::string, GLuint> m_uniformMap;
};

#endif  // TECHNIQUE_H

