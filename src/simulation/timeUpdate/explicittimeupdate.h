#ifndef EXPLICITTIMEUPDATE_H
#define EXPLICITTIMEUPDATE_H
#include "../timeUpdate.h"
class ExplicitTimeUpdate : public TimeUpdate
{
public:
    ExplicitTimeUpdate(std::shared_ptr<std::vector< Mesh >> const meshesToSimulate, shared_ptr<ParticleSystem> const particlesToSimulate, std::shared_ptr<Grid> const grid) : TimeUpdate(meshesToSimulate,particlesToSimulate, grid){

    }

    void update(double dt);
};

#endif // EXPLICITTIMEUPDATE_H
