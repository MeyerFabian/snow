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
  Shader(const ShaderType &type, const std::string &filename);
  ~Shader();
  void loadFromFile();
  void compile();
  void destroy();

  std::string getSource() const { return source; }
  std::string getFileName() const { return filename; }
  ShaderType getType() const { return type; }
  const GLuint getID() const { return id; }

 private:
  GLuint id;
  ShaderType type;
  std::string source;
  std::string filename;
};

#endif  // SHADER_H

