#ifndef SEMIIMPLICITTIMEUPDATE_H
#define SEMIIMPLICITTIMEUPDATE_H
#include "../timeUpdate.hpp"
#include "../../object/collisionObjects.hpp"
#include "../../object/grid.hpp"
class SemiImplicitTimeUpdate :public TimeUpdate
{
public:
    SemiImplicitTimeUpdate(std::shared_ptr<CollisionObjects> const collisonsToSimulate, shared_ptr<ParticleSystem> const particlesToSimulate, std::shared_ptr<Grid> const grid): TimeUpdate(collisonsToSimulate,particlesToSimulate, grid){

    }
    void init();
    void update(double dt);
};

#endif // SEMIIMPLICITTIMEUPDATE_H
