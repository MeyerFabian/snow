#ifndef SHADER_H
#define SHADER_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <sstream>
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

  void load_from_file();
  void upload();

  std::string get_source() const { return source; }
  std::string get_file_name() const { return filename; }
  ShaderType get_type() const { return type; }
  GLuint get_id() const { return id; }
  bool is_uploaded() const { return uploaded; }

 private:
  void gl_create_id();
  bool gl_compile();
  void gl_delete();

  ShaderType type;
  std::string filename;

  std::string source;
  GLuint id;
  bool uploaded = false;
};

#endif  // SHADER_H

