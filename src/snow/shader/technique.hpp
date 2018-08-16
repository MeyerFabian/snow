#ifndef TECHNIQUE_H
#define TECHNIQUE_H

#define GLEW_STATIC
#include <GL/glew.h>

#include <string.h>
#include <fstream>
#include <iostream>
#include <map>
#include <math3d.hpp>
#include <string>
#include <vector>
using namespace std;

class Technique {
 protected:
  GLuint ShaderProgram;

 private:
  vector<GLuint> ShaderObjects;
  std::map<std::string, GLuint> m_uniformMap;
  void gl_uniforms_read();

 public:
  void uniform_update(const std::string&, bool value) const;
  void uniform_update(const std::string&, int value) const;
  void uniform_update(const std::string&, float value) const;
  void uniform_update(const std::string&, double value) const;
  void uniform_update(const std::string&, float x, float y, float z) const;
  void uniform_update(const std::string&, int x, int y, int z) const;
  void uniform_update(const std::string&, const Matrix4f* mat4) const;
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

