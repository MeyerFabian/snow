#ifndef TIMEUPDATE_H
#define TIMEUPDATE_H

#include "../object/mesh.h"
#include "../object/particlesystem.h"
#include <memory>
#include "../object/grid.h"
#include "../object/collisionObjects.h"
class TimeUpdate{
public:
    TimeUpdate(std::shared_ptr<CollisionObjects> const collisionToSimulate, shared_ptr<ParticleSystem> const particlesToSimulate,shared_ptr<Grid> const gridToSimulate )
        : collisionObjects(collisionToSimulate), particlesystem(particlesToSimulate) , grid(gridToSimulate){

    }
    virtual void init() =0;
    virtual void update(double dt) = 0;

    std::shared_ptr<CollisionObjects> const  collisionObjects;
    std::shared_ptr<ParticleSystem> const  particlesystem;
    std::shared_ptr<Grid> const grid;
};

#endif // TIMEUPDATE_H
