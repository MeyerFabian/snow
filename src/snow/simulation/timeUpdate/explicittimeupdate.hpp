#ifndef EXPLICITTIMEUPDATE_H
#define EXPLICITTIMEUPDATE_H
#include "../timeUpdate.hpp"
#include "../technique/particleCompute.hpp"
#include "../technique/overGrid.hpp"
#include "../../defines/defines.hpp"
#include "../../object/collisionObjects.hpp"
#include "../../object/grid.hpp"
class ExplicitTimeUpdate : public TimeUpdate
{
public:
    ExplicitTimeUpdate(std::shared_ptr<CollisionObjects> const collisonsToSimulate, shared_ptr<ParticleSystem> const particlesToSimulate, std::shared_ptr<Grid> const grid) : TimeUpdate(collisonsToSimulate,particlesToSimulate, grid) {

    }
    void init();
    void update(double dt);
    ParticleCompute cVolume;
    ParticleCompute pc;
    ParticleCompute pVU;
    ParticleCompute cMass;
    OverGrid rg;
    OverGrid vUp;
    OverGrid pU;
    OverGrid rigidSim;
    //OverGrid divVelMass;
};

#endif // EXPLICITTIMEUPDATE_H
