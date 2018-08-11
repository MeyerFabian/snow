#ifndef TECHNIQUE_H
#define TECHNIQUE_H

#define GLEW_STATIC
#include <GL/glew.h>

#include <string.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "math3d.hpp"
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
  void readUniforms();

 public:
  void updateUniform(std::string name, bool value);
  void updateUniform(std::string name, int value);
  void updateUniform(std::string name, float value);
  void updateUniform(std::string name, double value);
  void updateUniform(std::string name, float x, float y, float z);
  void updateUniform(std::string name, int x, int y, int z);
  void updateUniform(std::string name, const size_t size);
  void updateUniform(std::string name, const Matrix4f* mat4);
  void use();
  void compile();

 private:
  GLuint uniformLookUp(const std::string&);
  std::map<std::string, int> m_uniformMap;
};

#endif  // TECHNIQUE_H

