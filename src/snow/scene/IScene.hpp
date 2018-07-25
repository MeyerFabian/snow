#ifndef ISCENE_H
#define ISCENE_H
#include "../object/collisionObjects.hpp"
#include "../object/mesh.hpp"
#include "../object/particlesystem.hpp"
class IScene {
 public:
  IScene() = default;
  virtual ~IScene() = default;
  virtual void init(shared_ptr<ParticleSystem> const pPs,
                    shared_ptr<CollisionObjects> const pCO,
                    shared_ptr<std::vector<shared_ptr<Mesh>>> const meshes) = 0;
};
#endif

