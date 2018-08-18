#ifndef MYPHYSICENGINE_H
#define MYPHYSICENGINE_H
#include "../object/grid.hpp"
#include "../simulation/physicEngine.hpp"
#include "timeUpdate.hpp"
#include "timeUpdate/explicittimeupdate.hpp"

class MPMPhysicEngine : public physicEngine {
 public:
  MPMPhysicEngine(std::unique_ptr<TimeUpdate> update)
      : physicEngine(std::move(update)) {}

  virtual bool init() override;
  virtual void update(double dt) override;
};

#endif  // MYPHYSICENGINE_H

