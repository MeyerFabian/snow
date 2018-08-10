#ifndef EXPLICITTIMEUPDATE_H
#define EXPLICITTIMEUPDATE_H
#include "../../defines/defines.hpp"
#include "../../object/collisionObjects.hpp"
#include "../../object/grid.hpp"
#include "../computingTechnique/g2pCompute.hpp"
#include "../computingTechnique/massCompute.hpp"
#include "../computingTechnique/p2gCompute.hpp"
#include "../computingTechnique/particleCompute.hpp"
#include "../computingTechnique/resetGridCompute.hpp"
#include "../computingTechnique/rigidCompute.hpp"
#include "../computingTechnique/velGridCompute.hpp"
#include "../computingTechnique/volumeCompute.hpp"
#include "../timeUpdate.hpp"
class ExplicitTimeUpdate : public TimeUpdate {
 public:
  ExplicitTimeUpdate(
      std::shared_ptr<CollisionObjects> const collisonsToSimulate,
      shared_ptr<ParticleSystem> const particlesToSimulate,
      std::shared_ptr<Grid> const grid)
      : TimeUpdate(collisonsToSimulate, particlesToSimulate, grid),
        cVolume(VolumeCompute()),
        p2g(P2GCompute()),
        g2p(G2PCompute()),
        cMass(MassCompute()),
        rg(ResetGridCompute()),
        g2g(VelGridCompute()),
        pU(ParticleCompute()),
        rigidSim(RigidCompute()) {}
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
};

#endif  // EXPLICITTIMEUPDATE_H

