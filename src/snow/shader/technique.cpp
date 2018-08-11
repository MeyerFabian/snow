#include "technique.hpp"

Technique::Technique() : shaderProgram(glCreateProgram()) {
  if (shaderProgram == 0) {
    fprintf(stderr, "Error creating shader program\n");
  }
}
Technique::~Technique() {
  glDeleteProgram(shaderProgram);
  std::cerr << "deleted program\n";
}
void Technique::compile() {
  for (auto& shaderObject : shaderObjects) {
    shaderObject->loadFromFile();
    shaderObject->compile();
  }
  finalize();
  readUniforms();
}
void Technique::use() { glUseProgram(shaderProgram); }

bool Technique::addShader(std::shared_ptr<Shader>&& shader) {
  auto new_shader_pos = std::find_if(
      shaderObjects.cbegin(), shaderObjects.cend(),
      [newtype = shader->getType()](const auto& shaderObject) {
        auto oldtype = shaderObject->getType();
        return (oldtype == newtype) || (oldtype == ShaderType::COMPUTE);
      });
  bool is_compute_technique =
      (shader->getType() == ShaderType::COMPUTE && !shaderObjects.empty());
  bool is_not_new_type = (new_shader_pos != shaderObjects.end());
  if (is_compute_technique || is_not_new_type) {
    std::cerr
        << "ILLEGAL ShaderType:\n You tried to add a type that is "
           "incompatible with the rest of the Technique.\n Only one ShaderType "
           "allowed. Except for ShaderType::COMPUTE is always alone:\n"
        << shader->getFileName() << "!\n";
    return false;
  } else {
    shaderObjects.push_back(std::move(shader));
  }
  return true;
}

void Technique::finalize() {
  std::cerr << "ShaderProgram:" << shaderProgram << std::endl;
  for (const auto& shaderObject : shaderObjects) {
    std::cerr << "Attach Shader: " << shaderObject->getFileName() << "\n";
    glAttachShader(this->shaderProgram, shaderObject->getID());
  }
  GLint Success = 0;
  GLchar ErrorLog[1024] = {0};
  glLinkProgram(shaderProgram);
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &Success);
  if (Success == 0) {
    glGetProgramInfoLog(shaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
    fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
  }

  glValidateProgram(shaderProgram);
  glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &Success);
  if (!Success) {
    glGetProgramInfoLog(shaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
    fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
  }
}
void Technique::readUniforms() {
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
GLuint Technique::uniformLookUp(const std::string& uniform) {
  // Create an iterator to look through our uniform map and try to find the
  // named uniform
  auto it = std::find_if(
      m_uniformMap.cbegin(), m_uniformMap.cend(),
      [uniform](const auto& mapEntry) { return uniform == mapEntry.first; });
  // Found it? Great - pass it back! Didn't find it? Alert user and halt.
  if (it != m_uniformMap.end()) {
    return m_uniformMap[uniform];
  } else {
    std::cerr << "Could not find uniform in shader program: " + uniform
              << std::endl;
    std::cerr << "Associated shaders with this shader program:" << std::endl;
    for (const auto& shaderObject : shaderObjects) {
      std::cerr << shaderObject->getFileName() << std::endl;
    }
    return 0;
  }
}

void Technique::updateUniform(std::string name, bool value) {
  use();
  glUniform1i(uniformLookUp(name), value);
}

void Technique::updateUniform(std::string name, int value) {
  use();
  glUniform1i(uniformLookUp(name), value);
}

void Technique::updateUniform(std::string name, float value) {
  use();
  glUniform1f(uniformLookUp(name), value);
}

void Technique::updateUniform(std::string name, double value) {
  use();
  glUniform1f(uniformLookUp(name), value);
}
void Technique::updateUniform(std::string name, float x, float y, float z) {
  use();
  glUniform3f(uniformLookUp(name), x, y, z);
}
void Technique::updateUniform(std::string name, int x, int y, int z) {
  use();
  glUniform3i(uniformLookUp(name), x, y, z);
}
void Technique::updateUniform(std::string name, const Matrix4f* mat4) {
  use();
  glUniformMatrix4fv(uniformLookUp(name), 1, GL_TRUE, (const GLfloat*)mat4);
}
void Technique::updateUniform(std::string name, const size_t size) {
  use();
  glUniform1ui(uniformLookUp(name), size);
}

