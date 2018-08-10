#include "shader.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

Shader::Shader(const ShaderType &t) : type(t) {
  // Get the type of the shader
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
      gl_type = GL_TESS_CONTROL_SHADER;
      break;
  }

  // Create the vertex shader id / handle
  // Note: If you segfault here you probably don't have a valid rendering
  // context.
  id = glCreateShader(gl_type);
}

Shader::~Shader() {}

void Shader::loadFromString(const std::string &sourceString) {
  // Keep hold of a copy of the source
  source = sourceString;

  // Get the source as a pointer to an array of characters
  const char *sourceChars = source.c_str();

  // Associate the source with the shader id
  glShaderSource(id, 1, &sourceChars, NULL);
}

void Shader::loadFromFile(const std::string &filename) {
  std::ifstream file;

  file.open(filename.c_str());

  if (!file.good()) {
    std::cerr << "could not open shader: " << filename.c_str() << std::endl;
    exit(-1);
  }

  // Create a string stream
  std::stringstream stream;

  // Dump the contents of the file into it
  stream << file.rdbuf();

  // Close the file
  file.close();

  // Convert the StringStream into a string
  source = stream.str();

  // Get the source string as a pointer to an array of characters
  const char *sourceChars = source.c_str();

  // Associate the source with the shader id
  glShaderSource(id, 1, &sourceChars, NULL);
}

void Shader::compile() {
  // Compile the shader
  glCompileShader(id);

  // Check the compilation status and report any errors
  GLint shaderStatus;
  glGetShaderiv(id, GL_COMPILE_STATUS, &shaderStatus);

  // If the shader failed to compile, display the info log and quit out
  if (shaderStatus == GL_FALSE) {
    GLint infoLogLength;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);

    GLchar *strInfoLog = new GLchar[infoLogLength + 1];
    glGetShaderInfoLog(id, infoLogLength, NULL, strInfoLog);

    std::cerr << "shader compilation failed: " << strInfoLog << std::endl;
    delete[] strInfoLog;
  }
}

