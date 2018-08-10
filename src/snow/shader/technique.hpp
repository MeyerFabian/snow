#ifndef TECHNIQUE_H
#define TECHNIQUE_H

#define GLEW_STATIC
#include <GL/glew.h>

#include <string.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "shader.hpp"
#define INVALID_UNIFORM_LOCATION 0xffffffff

class Technique {
 protected:
  Technique();
  ~Technique();
  GLuint shaderProgram;
  void finalize();
  bool addShader(std::shared_ptr<Shader>&& ptr);
  std::vector<std::shared_ptr<Shader>> shaderObjects;

 public:
  void use();
};

#endif  // TECHNIQUE_H

