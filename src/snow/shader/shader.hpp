#ifndef SHADER_H
#define SHADER_H
#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "../utils/file_loader.hpp"

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
  Shader(ShaderType type, const std::string &filename);
  ~Shader();

  void load_shader_from_file();
  void process_includes(std::string &file, unsigned long long start,
                        unsigned long long end_search);
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
  GLuint gl_map_type();

  ShaderType type;
  std::string filename;

  std::string source;
  GLuint id;
  bool uploaded = false;
};

#endif  // SHADER_H

