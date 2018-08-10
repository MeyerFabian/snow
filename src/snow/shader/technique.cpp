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
    shaderObjects.insert(new_shader_pos, std::move(shader));
  }
  return true;
}
void Technique::finalize() {
  for (const auto& shaderObject : shaderObjects) {
    std::cout << shaderObject->getID() << "\n";
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

