#include "shader.hpp"
Shader::Shader(const ShaderType &t, const std::string &filename)
    : type(t), filename(filename) {}

void Shader::load_from_file() {
  std::ifstream file;

  file.open(filename.c_str());
  if (!file.good()) {
    std::cerr << "could not open shader: " << filename.c_str() << std::endl;
    exit(-1);
  }

  std::stringstream stream;
  stream << file.rdbuf();
  file.close();
  source = stream.str();
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

void Shader::gl_create_id() {
  // We handle the GLuint type by scoped enums of C++14 which should be safer
  GLuint gl_type;
  switch (type) {
    case ShaderType::VERTEX:
      gl_type = GL_VERTEX_SHADER;
      break;
    case ShaderType::TESS_EVAL:
      gl_type = GL_TESS_EVALUATION_SHADER;
      break;
    case ShaderType::TESS_CNTRL:
      gl_type = GL_TESS_CONTROL_SHADER;
      break;
    case ShaderType::GEOMETRY:
      gl_type = GL_GEOMETRY_SHADER;
      break;
    case ShaderType::FRAGMENT:
      gl_type = GL_FRAGMENT_SHADER;
      break;
    case ShaderType::COMPUTE:
      gl_type = GL_COMPUTE_SHADER;
      break;
  }

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

