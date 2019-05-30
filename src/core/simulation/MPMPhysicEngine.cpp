#include "MPMPhysicEngine.hpp"

bool MPMPhysicEngine::init() {
  integration->init();
  return true;
}
void MPMPhysicEngine::update(double dt) { integration->update(dt); }

