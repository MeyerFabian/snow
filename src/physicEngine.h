#ifndef PHYSICENGINE_H
#define PHYSICENGINE_H
#include <memory>
#include <vector>
#include "object/mesh.h"
#include "object/particlesystem.h"

class physicEngine{
    public :
    physicEngine( std::shared_ptr<std::vector< Mesh >> const meshesToSimulate, shared_ptr<ParticleSystem> const particlesToSimulate): meshes(meshesToSimulate), particles(particlesToSimulate){}

    virtual bool init() = 0;
    virtual void update(double dt) = 0;

    std::shared_ptr<std::vector< Mesh >> const  meshes;
    std::shared_ptr<ParticleSystem> const  particles;
};

#endif // PHYSICENGINE_H
