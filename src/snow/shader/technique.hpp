#ifndef TECHNIQUE_H
#define TECHNIQUE_H

#define GLEW_STATIC
#include <GL/glew.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "math3d.hpp"
#include "shader.hpp"

class Technique {
 protected:
  Technique();
  virtual ~Technique();
  bool add_shader(std::shared_ptr<Shader>&& ptr);

 public:
  void uniform_update(std::string name, bool value) const;
  void uniform_update(std::string name, int value) const;
  void uniform_update(std::string name, float value) const;
  void uniform_update(std::string name, double value) const;
  void uniform_update(std::string name, float x, float y, float z) const;
  void uniform_update(std::string name, int x, int y, int z) const;
  void uniform_update(std::string name, const size_t size) const;
  void uniform_update(std::string name, const Matrix4f* mat4) const;
  void use() const;
  void upload();

 private:
  unsigned int uniform_look_up(std::string) const;

  void attach_and_link() const;
  void attach() const;
  void gl_link() const;
  void gl_attach(GLuint) const;

  void gl_uniforms_read();

  GLuint shaderProgram;
  std::vector<std::shared_ptr<Shader>> shaderObjects;
  std::map<std::string, unsigned int> m_uniformMap;
};

#endif  // TECHNIQUE_H

