#ifndef EXPLICITTIMEUPDATE_H
#define EXPLICITTIMEUPDATE_H
#include "../timeUpdate.h"
class ExplicitTimeUpdate : public TimeUpdate
{
public:
    ExplicitTimeUpdate(std::shared_ptr<std::vector< Mesh >> const meshesToSimulate, shared_ptr<ParticleSystem> const particlesToSimulate) : TimeUpdate(meshesToSimulate,particlesToSimulate){

    }

    void update(double dt);
};

#endif // EXPLICITTIMEUPDATE_H
