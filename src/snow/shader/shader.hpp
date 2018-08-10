#ifndef SHADER_H
#define SHADER_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

enum class ShaderType {
  VERTEX,
  TESS_EVAL,
  TESS_CNTRL,
  GEOMETRY,
  FRAGMENT,
  COMPUTE
};

class Shader {
 public:
  Shader(const ShaderType &type);
  ~Shader();

  void loadFromString(const std::string &sourceString);
  void loadFromFile(const std::string &filename);

  void compile();

  GLuint getId() { return id; }
  std::string getSource() { return source; }
  ShaderType getType() { return type; }

 private:
  GLuint id;
  ShaderType type;
  std::string source;
};

#endif  // SHADER_H

