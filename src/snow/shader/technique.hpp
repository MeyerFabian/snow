#ifndef TECHNIQUE_H
#define TECHNIQUE_H

#define GLEW_STATIC
#include <GL/glew.h>

#include <string.h>
#include <fstream>
#include <iostream>
#include <math3d.hpp>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

class Technique {
 protected:
  GLuint ShaderProgram;

 private:
  vector<GLuint> ShaderObjects;
  std::unordered_map<std::string, GLuint> m_uniformMap;
  void gl_uniforms_read();

 public:
  void uniform_update(const std::string&, const bool value) const noexcept;
  void uniform_update(const std::string&, const int value) const noexcept;
  void uniform_update(const std::string&, const float value) const noexcept;
  void uniform_update(const std::string&, const double value) const noexcept;
  void uniform_update(const std::string&, const float x, const float y,
                      const float z) const noexcept;
  void uniform_update(const std::string&, const int x, const int y,
                      const int z) const noexcept;
  void uniform_update(const std::string&, const Matrix4f* mat4) const noexcept;
  void init();
  Technique();
  ~Technique();
  GLuint getShaderProgram();
  void use();
  void addShader(const char* pShaderText, GLenum ShaderType);
  void upload();
};

bool ReadFile(const char* pFileName, string& outFile);

#endif  // TECHNIQUE_H

