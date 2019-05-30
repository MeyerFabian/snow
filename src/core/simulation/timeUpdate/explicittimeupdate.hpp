#ifndef EXPLICITTIMEUPDATE_H
#define EXPLICITTIMEUPDATE_H
#include "../../object/collision/collisionObjects.hpp"
#include "../../object/grid/grid.hpp"
#include "../../utils/benchmarker.hpp"
#include "../../utils/defines.hpp"
#include "../computingTechnique/g2pTransfer.hpp"
#include "../computingTechnique/gForceCompute.hpp"
#include "../computingTechnique/gMassCompute.hpp"
#include "../computingTechnique/gReset.hpp"
#include "../computingTechnique/p2gTransfer.hpp"
#include "../computingTechnique/pVolumeCompute.hpp"
#include "../computingTechnique/particleAdvance.hpp"
#include "../computingTechnique/rigidAdvance.hpp"
#include "../timeUpdate.hpp"
// TODO: needs some rework to use new pipeline
class ExplicitTimeUpdate : public TimeUpdate {
 public:
  ExplicitTimeUpdate(PhysicalScene&& sceneToSimulate)
      : TimeUpdate(std::move(sceneToSimulate)),
        compute_particle_volumes(PVolumeCompute()),
        transfer_to_grid(P2GTransfer()),
        transfer_back_to_particles(G2PTransfer()),
        compute_grid_mass(GMassCompute()),
        scratch_grid(GReset()),
        compute_grid_derivates(GForceCompute()),
        advance_particles(ParticleAdvance()),
        advance_rigids(RigidAdvance()),
        numParticles(sceneToSimulate.particleSys->particles.size()),
        numColliders(sceneToSimulate.colliderSys->colliders.size()) {}
  void init();
  void update(double dt);
  PVolumeCompute compute_particle_volumes;
  P2GTransfer transfer_to_grid;
  G2PTransfer transfer_back_to_particles;
  GMassCompute compute_grid_mass;
  GReset scratch_grid;
  GForceCompute compute_grid_derivates;
  ParticleAdvance advance_particles;
  RigidAdvance advance_rigids;

 private:
  // cached values
  const GLuint numParticles;
  const GLuint numColliders;
};

#endif  // EXPLICITTIMEUPDATE_H

