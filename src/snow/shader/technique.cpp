#include "technique.hpp"

Technique::Technique() {}
Technique::~Technique() {
  for (vector<GLuint>::iterator it = ShaderObjects.begin();
       it < ShaderObjects.end(); it++) {
    glDeleteShader(*it);
  }
  glDeleteProgram(ShaderProgram);
}
void Technique::init() {
  this->ShaderProgram = glCreateProgram();
  // glProgramParameteri(this->ShaderProgram , GL_PROGRAM_SEPARABLE, GL_TRUE);
  if (ShaderProgram == 0) {
    fprintf(stderr, "Error creating shader program\n");
  }
}

GLuint Technique::getShaderProgram() { return this->ShaderProgram; }
void Technique::plugTechnique() { glUseProgram(ShaderProgram); }

void Technique::addShader(const char* pShaderText, GLenum ShaderType) {
  GLuint ShaderObj = glCreateShader(ShaderType);
  const GLchar* p[1];
  p[0] = pShaderText;
  GLint Lengths[1];
  Lengths[0] = strlen(pShaderText);
  glShaderSource(ShaderObj, 1, p, Lengths);

  glCompileShader(ShaderObj);
  GLint success;
  glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
  if (!success) {
    GLchar InfoLog[1024];
    glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
    fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType,
            InfoLog);
  }
  ShaderObjects.push_back(ShaderObj);
  glAttachShader(this->ShaderProgram, ShaderObj);
}

void Technique::finalize() {
  GLint Success = 0;
  GLchar ErrorLog[1024] = {0};
  glLinkProgram(ShaderProgram);
  glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
  if (Success == 0) {
    glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
    fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
  }

  glValidateProgram(ShaderProgram);
  glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
  if (!Success) {
    glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
    fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
  }
  for (vector<GLuint>::iterator it = ShaderObjects.begin();
       it < ShaderObjects.end(); it++) {
    glDeleteShader(*it);
  }
  ShaderObjects.clear();
}

bool ReadFile(const char* pFileName, string& outFile) {
  ifstream f(pFileName);
  bool ret = false;
  if (f.is_open()) {
    string line;
    while (getline(f, line)) {
      outFile.append(line);
      outFile.append("\n");
    }
    f.close();
    ret = true;
  }
  return ret;
}
