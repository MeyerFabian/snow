#ifndef EXPLICITTIMEUPDATE_H
#define EXPLICITTIMEUPDATE_H
#include "../../object/collisionObjects.hpp"
#include "../../object/grid.hpp"
#include "../../utils/defines.hpp"
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
  ExplicitTimeUpdate(PhysicalScene&& sceneToSimulate)
      : TimeUpdate(std::move(sceneToSimulate)),
        cVolume(VolumeCompute()),
        p2g(P2GCompute()),
        g2p(G2PCompute()),
        cMass(MassCompute()),
        rg(ResetGridCompute()),
        g2g(VelGridCompute()),
        pU(ParticleCompute()),
        rigidSim(RigidCompute()),
        numParticles(sceneToSimulate.particleSys->particles.size()),
        numColliders(sceneToSimulate.colliderSys->colliders.size()) {}
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
  // cached values
  const GLuint numParticles;
  const GLuint numColliders;
};

#endif  // EXPLICITTIMEUPDATE_H

