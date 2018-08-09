#ifndef SCENE_H
#define SCENE_H
#include "../object/collisionObjects.hpp"
#include "../object/mesh.hpp"
#include "../object/particlesystem.hpp"

class Scene {
 public:
  Scene() = default;
  virtual void init(shared_ptr<ParticleSystem> const pPs,
                    shared_ptr<CollisionObjects> const pCO,
                    shared_ptr<std::vector<shared_ptr<Mesh>>> const meshes){};
};
#endif

