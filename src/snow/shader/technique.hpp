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
  void uniform_update(std::string name, bool value);
  void uniform_update(std::string name, int value);
  void uniform_update(std::string name, float value);
  void uniform_update(std::string name, double value);
  void uniform_update(std::string name, float x, float y, float z);
  void uniform_update(std::string name, int x, int y, int z);
  void uniform_update(std::string name, const Matrix4f* mat4);
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

