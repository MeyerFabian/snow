#ifndef SCENE_H
#define SCENE_H
#include "../object/collision/collisionObjects.hpp"
#include "../object/mesh.hpp"
#include "../object/particle/particlesystem.hpp"

struct PhysicalScene {
  std::shared_ptr<ParticleSystem> const particleSys;
  std::shared_ptr<Grid> grid;
  std::shared_ptr<CollisionObjects> const colliderSys;
};

struct RenderableScene {
  std::shared_ptr<ParticleSystem> const particleSys;
  std::shared_ptr<Grid> grid;
  std::shared_ptr<std::vector<shared_ptr<Mesh>>> const meshSys;
};

class Scene {
 public:
  std::shared_ptr<ParticleSystem> const particleSys =
      std::make_shared<ParticleSystem>();
  std::shared_ptr<Grid> grid = std::make_shared<Grid>();
  std::shared_ptr<CollisionObjects> const colliderSys =
      std::make_shared<CollisionObjects>();
  std::shared_ptr<std::vector<std::shared_ptr<Mesh>>> const meshSys =
      std::make_shared<std::vector<std::shared_ptr<Mesh>>>();
  RenderableScene get_renderable_scene() {
    return {particleSys, grid, meshSys};
  }
  PhysicalScene get_physical_scene() {
    return {particleSys, grid, colliderSys};
  }
};
#endif

