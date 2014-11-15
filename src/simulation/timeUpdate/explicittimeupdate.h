#ifndef EXPLICITTIMEUPDATE_H
#define EXPLICITTIMEUPDATE_H
#include "../timeUpdate.h"
#include "../technique/particleCompute.h"
class ExplicitTimeUpdate : public TimeUpdate
{
public:
    ExplicitTimeUpdate(std::shared_ptr<std::vector< Mesh >> const meshesToSimulate, shared_ptr<ParticleSystem> const particlesToSimulate, std::shared_ptr<Grid> const grid) : TimeUpdate(meshesToSimulate,particlesToSimulate, grid) {

    }
    void init();
    void update(double dt);
    ParticleCompute pc;
};

#endif // EXPLICITTIMEUPDATE_H
