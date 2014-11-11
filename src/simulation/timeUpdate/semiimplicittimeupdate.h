#ifndef SEMIIMPLICITTIMEUPDATE_H
#define SEMIIMPLICITTIMEUPDATE_H
#include "../timeUpdate.h"
class SemiImplicitTimeUpdate :public TimeUpdate
{
public:
    SemiImplicitTimeUpdate(std::shared_ptr<std::vector< Mesh >> const meshesToSimulate, shared_ptr<ParticleSystem> const particlesToSimulate): TimeUpdate(meshesToSimulate,particlesToSimulate){

    }

    void update(double dt);
};

#endif // SEMIIMPLICITTIMEUPDATE_H
