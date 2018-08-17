#include "technique.hpp"
void Technique::upload() {
  for (auto& shaderObject : shaderObjects) {
    if (!shaderObject->is_uploaded()) {
      shaderObject->load_from_file();
      shaderObject->upload();
    }
  }
  attach_and_link();
  gl_uniforms_read();
}

bool Technique::add_shader(std::shared_ptr<Shader>&& shader) {
  auto new_shader_pos = std::find_if(
      shaderObjects.cbegin(), shaderObjects.cend(),
      [newtype = shader->get_type()](const auto& shaderObject) {
        auto oldtype = shaderObject->get_type();
        return (oldtype == newtype) || (oldtype == ShaderType::COMPUTE);
      });

  bool isComputeTechnique =
      (shader->get_type() == ShaderType::COMPUTE && !shaderObjects.empty());
  bool isNotNewType = (new_shader_pos != shaderObjects.end());

  if (isComputeTechnique || isNotNewType) {
    std::cerr
        << "ILLEGAL ShaderType:\n You tried to add a type that is "
           "incompatible with the rest of the Technique.\n Only one ShaderType "
           "allowed. Except for ShaderType::COMPUTE is always alone:\n"
        << shader->get_file_name() << "!" << std::endl;
    return false;
  } else {
    shaderObjects.push_back(std::move(shader));
  }
  return true;
}
void Technique::attach_and_link() const {
  attach();
  gl_link();
}
void Technique::attach() const {
  std::cerr << "ShaderProgram:" << shaderProgram << std::endl;
  for (const auto& shaderObject : shaderObjects) {
    std::cerr << "Attach Shader: " << shaderObject->get_file_name() << "\n";
    gl_attach(shaderObject->get_id());
  }
}
unsigned int Technique::uniform_look_up(std::string uniform) const {
  // Create an iterator to look through our uniform map and try to find the
  // named uniform
  auto it = std::find_if(
      m_uniformMap.cbegin(), m_uniformMap.cend(),
      [uniform](const auto& mapEntry) { return uniform == mapEntry.first; });
  // Found it? Great - pass it back! Didn't find it? Alert user and halt.
  if (it != m_uniformMap.end()) {
    return m_uniformMap.at(uniform);
  } else {
    std::cerr << "Could not find uniform in shader program: " + uniform
              << std::endl;
    std::cerr << "Associated shaders with this shader program:" << std::endl;
    for (const auto& shaderObject : shaderObjects) {
      std::cerr << shaderObject->get_file_name() << std::endl;
    }
    return 0;
  }
}

/*
 * start of glFunction() calls
 */

void Technique::use() const { glUseProgram(shaderProgram); }

void Technique::gl_attach(GLuint shaderid) const {
  glAttachShader(shaderProgram, shaderid);
}

void Technique::gl_link() const {
  GLint Success = 0;
  GLchar ErrorLog[1024] = {0};
  glLinkProgram(shaderProgram);
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &Success);
  if (Success == 0) {
    glGetProgramInfoLog(shaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
    std::cerr << "Error linking shader program: " << ErrorLog << std::endl;
  }

  glValidateProgram(shaderProgram);
  glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &Success);
  if (!Success) {
    glGetProgramInfoLog(shaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
    std::cerr << "Invalid shader program: " << ErrorLog << std::endl;
  }
}
void Technique::gl_uniforms_read() {
  GLint numUniforms = -1;
  glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORMS, &numUniforms);

  std::cerr << "Number of uniforms " + std::to_string(numUniforms) << std::endl;

  for (int i = 0; i < numUniforms; i++) {
    // passive variables for glGetActiveUniform
    int nameLength = -1;
    int uniformSize = -1;
    GLenum type = GL_ZERO;
    // string that saves uniformName
    char uniformName[50];
    glGetActiveUniform(shaderProgram, GLint(i), sizeof(uniformName) - 1,
                       &nameLength, &uniformSize, &type, uniformName);
    uniformName[nameLength] = 0;
    // add uniform variable to map
    m_uniformMap[uniformName] =
        glGetUniformLocation(shaderProgram, uniformName);
    std::cerr << "Uniform added " + std::to_string(i) + " : " + uniformName
              << std::endl;
  }
}
Technique::Technique() : shaderProgram(glCreateProgram()) {
  if (shaderProgram == 0) {
    std::cerr << "Error creating shader program" << std::endl;
  }
}
Technique::~Technique() {
  glDeleteProgram(shaderProgram);
  std::cerr << "deleted program" << std::endl;
}

void Technique::uniform_update(std::string name, bool value) const {
  glUniform1i(uniform_look_up(name), value);
}

void Technique::uniform_update(std::string name, int value) const {
  glUniform1i(uniform_look_up(name), value);
}

void Technique::uniform_update(std::string name, float value) const {
  glUniform1f(uniform_look_up(name), value);
}

void Technique::uniform_update(std::string name, double value) const {
  glUniform1f(uniform_look_up(name), value);
}
void Technique::uniform_update(std::string name, float x, float y,
                               float z) const {
  glUniform3f(uniform_look_up(name), x, y, z);
}
void Technique::uniform_update(std::string name, double x, double y,
                               double z) const {
  glUniform3f(uniform_look_up(name), x, y, z);
}
void Technique::uniform_update(std::string name, int x, int y, int z) const {
  glUniform3i(uniform_look_up(name), x, y, z);
}
void Technique::uniform_update(std::string name, const Matrix4f* mat4) const {
  glUniformMatrix4fv(uniform_look_up(name), 1, GL_TRUE, (const GLfloat*)mat4);
}
void Technique::uniform_update(std::string name, unsigned int size) const {
  glUniform1ui(uniform_look_up(name), size);
}

