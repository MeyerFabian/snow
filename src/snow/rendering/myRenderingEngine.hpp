#ifndef MYRENDERINGENGINE_H
#define MYRENDERINGENGINE_H
#include "../rendering/renderingEngine.hpp"

#define GLEW_STATIC
#include <GL/glew.h>
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
#include "math3d.hpp"
#include "pipeline/pipeline.hpp"
#include "stb_image.h"
#include "technique/lightingtechnique.hpp"
#include "technique/particletechnique.hpp"
#include "technique/shadowMapTechnique.hpp"
#include "technique/shadowmapbufferobject.hpp"
using namespace std;
class myRenderingEngine : public renderingEngine {
 public:
  myRenderingEngine(
      std::shared_ptr<std::vector<shared_ptr<Mesh> > > const meshes,
      std::shared_ptr<ParticleSystem> const particles,
      std::shared_ptr<Grid> const grid)
      : renderingEngine(meshes, particles, grid) {}
  virtual bool init();
  virtual void render();
  virtual bool shouldClose();
  virtual void stop();
  LightingTechnique lighting;
  ParticleTechnique particleImposter;
  ParticleTechnique gridBorderLines;
  void fillBufferFromMeshes();
  void initVBO();
  void initShader();
  void shadowMapPass();
  void renderPass();
  void renderQueue();
};
#endif  // MYRENDERINGENGINE_H

