#ifndef TECHNIQUE_H
#define TECHNIQUE_H

#define GLEW_STATIC
#include <GL/glew.h>

#include <string.h>
#include <fstream>
#include <string>
#include <vector>

#define INVALID_UNIFORM_LOCATION 0xffffffff
using namespace std;

class Technique {
 protected:
  GLuint ShaderProgram;

 private:
  vector<GLuint> ShaderObjects;

 public:
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
