#ifndef SHADER_H
#define SHADER_H
#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <algorithm>
#include <array>
#include <vector>
#include "../utils/defines.hpp"
#include "../utils/file_loader.hpp"
enum class ShaderType {
  VERTEX,
  TESS_EVAL,
  TESS_CNTRL,
  GEOMETRY,
  FRAGMENT,
  COMPUTE
};
struct LocalSize {
  GLuint x = 1;
  GLuint y = 1;
  GLuint z = 1;
};
enum PreprocessorCmd { DEFINE, INCLUDE };

class Shader {
 public:
  Shader(ShaderType type, const std::string &filename);
  ~Shader();

  void load_shader_from_file();
  void process_includes(std::string &file, unsigned long long start,
                        unsigned long long end_search,
                        std::string filename_tagged);
  void upload();

  std::string get_source() const { return source; }
  std::string get_file_name() const { return filename; }
  ShaderType get_type() const { return type; }
  GLuint get_id() const { return id; }
  bool is_uploaded() const { return uploaded; }

  std::vector<std::string> preprocess_cmds;

  using CommandType = std::pair<PreprocessorCmd, std::string>;
  template <typename Iterator>
  void add_cmds(Iterator begin, Iterator end) {
    // expects CommandType as type iterated over
    static_assert(
        std::is_same<typename std::iterator_traits<Iterator>::value_type,
                     CommandType>::value,
        "Not CommandType");

    std::transform(begin, end, std::back_inserter(preprocess_cmds),
                   [](const auto &pair_cmd) {
                     std::string cmd;
                     switch (pair_cmd.first) {
                       case PreprocessorCmd::DEFINE:
                         cmd += "#define ";
                         break;
                       case PreprocessorCmd::INCLUDE:
                         cmd += "#include ";
                         break;
                     }
                     return cmd + pair_cmd.second;
                   });
  }
  void set_local_size(const LocalSize &c);

 private:
  void gl_create_id();
  bool gl_compile();
  void gl_delete();
  GLuint gl_map_type();
  void add_local_size();
  void add_prec_include();
  void add_prec_define();
  void add_aos_define();
  void add_access_include();
  void add_glsl_define();

  ShaderType type;
  std::string filename;
  LocalSize local_size;

  std::string source;
  GLuint id;
  bool uploaded = false;
};

#endif  // SHADER_H

