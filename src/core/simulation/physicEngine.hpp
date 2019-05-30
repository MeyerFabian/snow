#ifndef PHYSICENGINE_H
#define PHYSICENGINE_H
#include <memory>
#include <vector>
#include "timeUpdate.hpp"

class physicEngine {
 public:
  physicEngine(std::unique_ptr<TimeUpdate> update)
      : integration(std::move(update)) {}

  virtual bool init() = 0;
  virtual void update(double dt) = 0;

  std::unique_ptr<TimeUpdate> integration;
};

#endif  // PHYSICENGINE_H

