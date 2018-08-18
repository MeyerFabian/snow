#ifndef RENDERER_H
#define RENDERER_H
#include <memory>
#include <vector>
#include "../object/grid.hpp"
#include "../object/mesh.hpp"
#include "../object/particlesystem.hpp"
#include "../scene/Scene.hpp"
class Renderer {
 public:
  Renderer(RenderableScene&& sceneToRender) : scene(std::move(sceneToRender)) {}

  virtual bool init() = 0;
  virtual void render() = 0;
  virtual bool shouldClose() = 0;
  virtual void stop() = 0;

  const RenderableScene scene;
};

#endif  // RENDERER_H

