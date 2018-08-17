#ifndef MYRENDERINGENGINE_H
#define MYRENDERINGENGINE_H
#include "GLFWWrapper.hpp"

#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glm.hpp>
#include <iostream>
#include <memory>
#include "../defines/defines.hpp"
#include "../object/mesh.hpp"
#include "../object/texture.hpp"
#include "../rendering/renderingEngine.hpp"
#include "math3d.hpp"
#include "pipeline/pipeline.hpp"
#include "stb_image.h"
#include "technique/lightingtechnique.hpp"
#include "technique/particletechnique.hpp"
#include "technique/shadowMapTechnique.hpp"
#include "technique/shadowmapbufferobject.hpp"
class myRenderingEngine : public renderingEngine {
 public:
  myRenderingEngine(
      std::shared_ptr<std::vector<shared_ptr<Mesh> > > const meshes,
      std::shared_ptr<ParticleSystem> const particles,
      std::shared_ptr<Grid> const grid)
      : renderingEngine(meshes, particles, grid), glfwContext(GLFWWrapper()) {}
  virtual bool init();
  virtual void render();
  virtual bool shouldClose();
  virtual void stop();

  void fillBufferFromMeshes();
  void initVBO();
  void initShader();
  void shadowMapPass();
  void renderPass();
  void renderQueue();

 private:
  GLuint VBO;
  GLuint IBO;
  GLFWWrapper glfwContext;
  LightingTechnique lighting;
  ParticleTechnique particleImposter;
  ParticleTechnique gridBorderLines;
};
#endif  // MYRENDERINGENGINE_H

