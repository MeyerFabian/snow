#ifndef TECHNIQUE_H
#define TECHNIQUE_H

#define GLEW_STATIC
#include <GL/glew.h>

#include <string.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include "math3d.hpp"
#define INVALID_UNIFORM_LOCATION 0xffffffff
using namespace std;

class Technique {
 protected:
  GLuint ShaderProgram;

 private:
  vector<GLuint> ShaderObjects;
  std::unordered_map<std::string, GLuint> m_uniformMap;
  void gl_uniforms_read();

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
  Technique();
  ~Technique();
  GLuint getShaderProgram();
  void plugTechnique();
  void addShader(const char* pShaderText, GLenum ShaderType);
  void finalize();
};

bool ReadFile(const char* pFileName, string& outFile);

#endif  // TECHNIQUE_H

