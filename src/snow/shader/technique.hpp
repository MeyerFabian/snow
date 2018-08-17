#ifndef TECHNIQUE_H
#define TECHNIQUE_H

#define GLEW_STATIC
#include <GL/glew.h>

#include <string.h>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

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
  void uniform_update(const std::string&, float) const;
  void uniform_update(const std::string&, int) const;
  void uniform_update(const std::string&, double) const;
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

