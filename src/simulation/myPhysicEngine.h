#ifndef MYPHYSICENGINE_H
#define MYPHYSICENGINE_H
#include "../physicEngine.h"
#include "timeUpdate.h"
class myPhysicEngine : public physicEngine
{
public:
    myPhysicEngine(std::shared_ptr<std::vector< Mesh >> const meshes, shared_ptr<ParticleSystem> const particles):physicEngine(meshes, particles){

    }

    virtual bool init();
    virtual void update(double dt);
};

#endif // MYPHYSICENGINE_H
