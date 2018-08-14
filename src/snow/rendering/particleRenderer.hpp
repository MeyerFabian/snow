#ifndef PARTICLERENDERER_H
#define PARTICLERENDERER_H
#include "../rendering/renderer.hpp"
#define GLEW_STATIC
#include <stdlib.h>
#include <string.h>
#include <glm.hpp>
#include <iostream>
#include <memory>
#include "../object/mesh.hpp"
#include "../object/texture.hpp"
#include "GLFWContext.h"
#include "math3d.hpp"
#include "renderingTechnique/lightingtechnique.hpp"
#include "renderingTechnique/particletechnique.hpp"
#include "renderingTechnique/shadowMapTechnique.hpp"
#include "renderingTechnique/shadowmapbufferobject.hpp"
#include "stb_image.h"

class ParticleRenderer : public Window_Context, public Renderer {
 public:
  ParticleRenderer(RenderableScene&&);
  virtual bool init() override;
  virtual void render() override;
  virtual bool shouldClose() override;
  virtual void stop() override;
  LightingTechnique lighting;
  ParticleTechnique particleImposter;
  ParticleTechnique gridBorderLines;
  GLuint VBO;
  GLuint IBO;
  void fillBufferFromMeshes();
  void initVBO();
  void shadowMapPass();
  void initShader();
  void renderPass();
  void renderQueue();
};
#endif  // PARTICLERENDERER_H

