#ifndef SEMIIMPLICITTIMEUPDATE_H
#define SEMIIMPLICITTIMEUPDATE_H
#include "../../object/collision/collisionObjects.hpp"
#include "../../object/grid/grid.hpp"
#include "../timeUpdate.hpp"
class SemiImplicitTimeUpdate : public TimeUpdate {
 public:
  SemiImplicitTimeUpdate(PhysicalScene&& scene)
      : TimeUpdate(std::move(scene)) {}
  void init();
  void update(double dt);
};

#endif  // SEMIIMPLICITTIMEUPDATE_H

