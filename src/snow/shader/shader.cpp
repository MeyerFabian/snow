#include "shader.hpp"
Shader::Shader(ShaderType t, const std::string &filename)
    : type(t), filename(filename) {
  add_glsl_define();
  if (t == ShaderType::COMPUTE) {
  }
#ifdef DOUBLE_PREC
  add_prec_define();
#endif
  add_prec_include();
}
void Shader::set_local_size(const LocalSize &w) { local_size = w; }

void Shader::add_local_size() {
  std::vector<CommandType> preprocess = {
      {PreprocessorCmd::DEFINE, "X " + std::to_string(local_size.x)},
      {PreprocessorCmd::DEFINE, "Y " + std::to_string(local_size.y)},
      {PreprocessorCmd::DEFINE, "Z " + std::to_string(local_size.z)}};
  add_cmds(preprocess.begin(), preprocess.end());
}

void Shader::add_prec_include() {
  auto cmd_prec = {
      CommandType(PreprocessorCmd::INCLUDE,
                  "\"shader/shared_hpp/precision.hpp\""),
  };
  add_cmds(cmd_prec.begin(), cmd_prec.end());
}

// adds #define GLSL mostly for includes of .hpp files that need to work
// slightly different in the shader
void Shader::add_glsl_define() {
  auto cmd_prec = {
      CommandType(PreprocessorCmd::DEFINE, "GLSL"),
  };
  add_cmds(cmd_prec.begin(), cmd_prec.end());
}

void Shader::add_prec_define() {
  auto cmd_prec = {
      CommandType(PreprocessorCmd::DEFINE, "DOUBLE_PREC"),
  };
  add_cmds(cmd_prec.begin(), cmd_prec.end());
}
void Shader::add_aos_define(BufferLayout layout) {
  if (layout == BufferLayout::AOS) {
    auto cmd_prec = {
        CommandType(PreprocessorCmd::DEFINE, "AOS_LAYOUT"),
    };
    add_cmds(cmd_prec.begin(), cmd_prec.end());
  }
}

void Shader::add_n_define(GLuint n) {
  auto cmd_prec = {
      CommandType(PreprocessorCmd::DEFINE, "N " + std::to_string(n)),
  };
  add_cmds(cmd_prec.begin(), cmd_prec.end());
}
void Shader::add_access_include() {
  auto cmd_access = {
      CommandType(PreprocessorCmd::INCLUDE,
                  "\"shader/utils/access.include.glsl\""),
  };
  add_cmds(cmd_access.begin(), cmd_access.end());
}

void Shader::load_shader_from_file() {
  const auto ext_start = filename.find(".");
  auto filename_tagged = filename;
  filename_tagged.insert(ext_start, "_compiled");
  const auto path_end = filename.find_last_of("/");
  filename_tagged.insert(path_end + 1, "compiled/");

  auto shdr_source = FileSystem::load_string_from_file(filename);

  const auto start = 0;

  const auto version_end = shdr_source.find("#version");
  const auto extension_end = shdr_source.rfind("#extension");
  auto important_end = version_end;
  if (extension_end != std::string::npos) {
    important_end = std::max(version_end, extension_end);
  }
  const auto preprocess_cmds_start = shdr_source.find('\n', important_end + 1);

  if (type == ShaderType::COMPUTE) {
    // Local size of the shader: "#define X (int)" will always be defined.
    add_local_size();
  }

  add_access_include();
  // Add additional commands (#define,#include, ..)
  std::string cmds_concat = "\n\n";
  for (const auto &cmd : preprocess_cmds) {
    cmds_concat += cmd + "\n";
  }
  shdr_source.insert(preprocess_cmds_start, cmds_concat);

  // Handle all #include commands
  const auto main_start = shdr_source.find("void main");
  process_includes(shdr_source, start, main_start, filename_tagged);

  FileSystem::write_to_file(shdr_source, filename_tagged);
  source = shdr_source;
}

void Shader::process_includes(std::string &file, unsigned long long start,
                              unsigned long long end_search,
                              std::string filename_tagged) {
  while ((start = file.find("#include", start)) <= end_search) {
    // file path for includes is within quotation marks
    const auto path_start = file.find('\"', start);
    const auto path_end = file.find('\"', path_start + 1);
    auto path_inc = file.substr(path_start + 1, path_end - path_start - 1);
    std::cout << "Including " << path_inc << "\n";

    FileSystem::write_to_file(file, filename_tagged);
    auto inc = FileSystem::load_string_from_file(path_inc);
    const auto eol = file.find('\n', path_end);
    // nested includes
    process_includes(inc, 0, inc.size(), filename_tagged);

    // replace source of file with include pragma
    file.replace(start, eol - start, inc.data());
    end_search += inc.size();
  }
}

void Shader::upload() {
  gl_create_id();
  if (!gl_compile()) {  // if shader could'nt get compiled
    gl_delete();        // delete shader handle
  };
}

/*
 * start of glFunction() calls
 */

Shader::~Shader() {}
void Shader::gl_delete() {
  if (uploaded) {
    glDeleteShader(id);
    uploaded = false;
  }
}
GLuint Shader::gl_map_type() {
  switch (type) {
    case ShaderType::VERTEX:
      return GL_VERTEX_SHADER;
      break;
    case ShaderType::TESS_EVAL:
      return GL_TESS_EVALUATION_SHADER;
      break;
    case ShaderType::TESS_CNTRL:
      return GL_TESS_CONTROL_SHADER;
      break;
    case ShaderType::GEOMETRY:
      return GL_GEOMETRY_SHADER;
      break;
    case ShaderType::FRAGMENT:
      return GL_FRAGMENT_SHADER;
      break;
    case ShaderType::COMPUTE:
      return GL_COMPUTE_SHADER;
      break;
  }
}
void Shader::gl_create_id() {
  // We handle the GLuint type by scoped enums of C++14 which should be safer
  GLuint gl_type = gl_map_type();
  // Note: If you segfault here you probably don't have a valid rendering
  // context and uploaded is not gonna get set.
  id = glCreateShader(gl_type);
  uploaded = true;
}

bool Shader::gl_compile() {
  auto sourceChars = source.c_str();

  glShaderSource(id, 1, &sourceChars, NULL);

  glCompileShader(id);

  GLint shaderStatus;
  glGetShaderiv(id, GL_COMPILE_STATUS, &shaderStatus);

  // If the shader failed to compile, display the info log and quit out
  bool compiled = (shaderStatus == GL_TRUE);
  if (!compiled) {
    GLint infoLogLength;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);

    GLchar *strInfoLog = new GLchar[infoLogLength + 1];
    glGetShaderInfoLog(id, infoLogLength, NULL, strInfoLog);

    std::cerr << "shader compilation failed: " << strInfoLog << std::endl;
    delete[] strInfoLog;
  }
  return compiled;
}

