#ifndef TIMEUPDATE_H
#define TIMEUPDATE_H
#include "../object/mesh.h"
#include "../object/particlesystem.h"
#include <memory>
#include "../object/grid.h"
class TimeUpdate{
public:
    TimeUpdate(std::shared_ptr<std::vector< Mesh >> const meshesToSimulate, shared_ptr<ParticleSystem> const particlesToSimulate,shared_ptr<Grid> const gridToSimulate )
        : meshes(meshesToSimulate), particlesystem(particlesToSimulate) , grid(gridToSimulate){

    }

    virtual void update(double dt) = 0;

    std::shared_ptr<std::vector< Mesh >> const  meshes;
    std::shared_ptr<ParticleSystem> const  particlesystem;
    std::shared_ptr<Grid> const grid;
};

#endif // TIMEUPDATE_H
