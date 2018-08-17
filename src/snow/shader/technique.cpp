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
  const char* p = pShaderText;
  GLint Lengths[1];
  Lengths[0] = strlen(pShaderText);
  glShaderSource(ShaderObj, 1, &p, NULL);

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
}

void Technique::finalize() {
  for (const auto& ShaderObject : ShaderObjects) {
    glAttachShader(this->ShaderProgram, ShaderObject);
  }
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
  gl_uniforms_read();
  for (const auto& ShaderObject : ShaderObjects) {
    glDeleteShader(ShaderObject);
  }
  ShaderObjects.clear();
}

void Technique::gl_uniforms_read() {
  GLint numUniforms = -1;
  glGetProgramiv(ShaderProgram, GL_ACTIVE_UNIFORMS, &numUniforms);

  std::cerr << "Number of uniforms " + std::to_string(numUniforms) << std::endl;

  for (int i = 0; i < numUniforms; i++) {
    // passive variables for glGetActiveUniform
    int nameLength = -1;
    int uniformSize = -1;
    GLenum type = GL_ZERO;
    // string that saves uniformName
    char uniformName[50];
    glGetActiveUniform(ShaderProgram, GLint(i), sizeof(uniformName) - 1,
                       &nameLength, &uniformSize, &type, uniformName);
    uniformName[nameLength] = 0;
    // add uniform variable to map
    m_uniformMap[uniformName] =
        glGetUniformLocation(ShaderProgram, uniformName);
    std::cerr << "Uniform added " + std::to_string(i) + " : " + uniformName
              << std::endl;
  }
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

void Technique::uniform_update(const std::string& name, float value) const
    noexcept {
  glUniform1f(m_uniformMap.at(name), value);
  // std::cerr << name << "calls float" << std::endl;
}

void Technique::uniform_update(const std::string& name, int value) const
    noexcept {
  glUniform1i(m_uniformMap.at(name), value);
  // std::cerr << name << "calls int" << std::endl;
}
void Technique::uniform_update(const std::string& name, double value) const
    noexcept {
  glUniform1f(m_uniformMap.at(name), value);
  // std::cerr << name << "calls double" << std::endl;
}
void Technique::uniform_update(const std::string& name, float x, float y,
                               float z) const noexcept {
  glUniform3f(m_uniformMap.at(name), x, y, z);
  // std::cerr << name << "calls 3f" << std::endl;
}
void Technique::uniform_update(const std::string& name, int x, int y,
                               int z) const noexcept {
  glUniform3i(m_uniformMap.at(name), x, y, z);
  // std::cerr << name << "calls 3i" << std::endl;
}

