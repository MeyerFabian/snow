#ifndef MPMPHYSICENGINE_H
#define MPMPHYSICENGINE_H
#include "../object/grid/grid.hpp"
#include "physicEngine.hpp"
#include "timeUpdate.hpp"
#include "timeUpdate/explicittimeupdate.hpp"

class MPMPhysicEngine : public physicEngine {
 public:
  MPMPhysicEngine(std::unique_ptr<TimeUpdate> update)
      : physicEngine(std::move(update)) {}

  virtual bool init() override;
  virtual void update(double dt) override;
};

#endif  // MPMPHYSICENGINE_H

