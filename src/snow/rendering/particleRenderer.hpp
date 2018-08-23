#ifndef PARTICLERENDERER_H
#define PARTICLERENDERER_H
#include <stdlib.h>
#include <string.h>
#include <glm.hpp>
#include <iostream>
#include <memory>
#include "../object/mesh.hpp"
#include "../object/texture.hpp"
#include "../rendering/renderer.hpp"
#include "math3d.hpp"
#include "pipeline/pipeline.hpp"
#include "renderingTechnique/gBorders.hpp"
#include "renderingTechnique/pPoints.hpp"
#include "renderingTechnique/phong.hpp"
#include "renderingTechnique/shadowMapTechnique.hpp"
#include "renderingTechnique/shadowmapbufferobject.hpp"
#include "stb_image.h"

class ParticleRenderer : public Renderer {
 public:
  ParticleRenderer(RenderableScene&&);

  static pipeline world;
  virtual void init() override;
  virtual void render() override;
  Phong basicLighting;
  PPoints particleImposter;
  GBorders gridBorderLines;
  GLuint VBO;
  GLuint IBO;
  void fillBufferFromMeshes();
  void initVBO();
  void shadowMapPass();
  void initShader();
  void renderPass();
};
#endif  // PARTICLERENDERER_H

