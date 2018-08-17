#ifndef EXPLICITTIMEUPDATE_H
#define EXPLICITTIMEUPDATE_H
#include "../../defines/defines.hpp"
#include "../../object/collisionObjects.hpp"
#include "../../object/grid.hpp"
#include "../technique/g2pCompute.hpp"
#include "../technique/massCompute.hpp"
#include "../technique/p2gCompute.hpp"
#include "../technique/particleCompute.hpp"
#include "../technique/resetGridCompute.hpp"
#include "../technique/rigidCompute.hpp"
#include "../technique/velGridCompute.hpp"
#include "../technique/volumeCompute.hpp"
#include "../timeUpdate.hpp"
class ExplicitTimeUpdate : public TimeUpdate {
 public:
  ExplicitTimeUpdate(
      std::shared_ptr<CollisionObjects> const collisonsToSimulate,
      shared_ptr<ParticleSystem> const particlesToSimulate,
      std::shared_ptr<Grid> const grid)
      : TimeUpdate(collisonsToSimulate, particlesToSimulate, grid),
        numColliders(collisonsToSimulate->colliders->size()),
        numParticles(particlesToSimulate->particles->size()) {}
  void init();
  void update(double dt);
  VolumeCompute cVolume;
  P2GCompute p2g;
  G2PCompute g2p;
  MassCompute cMass;
  ResetGridCompute rg;
  VelGridCompute g2g;
  ParticleCompute pU;
  RigidCompute rigidSim;

 private:
  const GLuint numColliders;
  const GLuint numParticles;
};

#endif  // EXPLICITTIMEUPDATE_H

