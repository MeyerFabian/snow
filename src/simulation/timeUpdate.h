#ifndef TIMEUPDATE_H
#define TIMEUPDATE_H
#include "../object/mesh.h"
#include "../object/particlesystem.h"
#include <memory>
class TimeUpdate{
public:
    TimeUpdate(std::shared_ptr<std::vector< Mesh >> const meshesToSimulate, shared_ptr<ParticleSystem> const particlesToSimulate)
        : meshes(meshesToSimulate), particlesystem(particlesToSimulate){

    }

    virtual void update(double dt) = 0;

    std::shared_ptr<std::vector< Mesh >> const  meshes;
    std::shared_ptr<ParticleSystem> const  particlesystem;
};

#endif // TIMEUPDATE_H
