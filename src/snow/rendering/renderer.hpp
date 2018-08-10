#ifndef RENDERER_H
#define RENDERER_H
#include <memory>
#include <vector>
#include "../object/grid.hpp"
#include "../object/mesh.hpp"
#include "../object/particlesystem.hpp"
class Renderer {
 public:
  Renderer(std::shared_ptr<std::vector<shared_ptr<Mesh>>> const meshesToRender,
           shared_ptr<ParticleSystem> const particlesToRender,
           shared_ptr<Grid> const gridToRender)
      : meshes(meshesToRender),
        particlesystem(particlesToRender),
        grid(gridToRender) {}

  virtual bool init() = 0;
  virtual void render() = 0;
  virtual bool shouldClose() = 0;
  virtual void stop() = 0;

  std::shared_ptr<std::vector<shared_ptr<Mesh>>> const meshes;
  std::shared_ptr<ParticleSystem> const particlesystem;
  std::shared_ptr<Grid> const grid;
};

#endif  // RENDERER_H

