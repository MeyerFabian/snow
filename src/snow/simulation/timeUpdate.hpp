#ifndef TIMEUPDATE_H
#define TIMEUPDATE_H

#include <memory>
#include "../object/collisionObjects.hpp"
#include "../object/grid.hpp"
#include "../object/mesh.hpp"
#include "../object/particlesystem.hpp"
#include "../scene/Scene.hpp"
class TimeUpdate {
 public:
  TimeUpdate(PhysicalScene&& sceneToSimulate)
      : scene(std::move(sceneToSimulate)) {}
  virtual void init() = 0;
  virtual void update(double dt) = 0;

  const PhysicalScene scene;
};

#endif  // TIMEUPDATE_H

