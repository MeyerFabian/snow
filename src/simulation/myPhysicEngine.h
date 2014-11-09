#ifndef MYPHYSICENGINE_H
#define MYPHYSICENGINE_H
#include "../physicEngine.h"
#include "timeUpdate.h"
class myPhysicEngine : public physicEngine
{
public:
    myPhysicEngine() = default;
    virtual bool init(std::shared_ptr<std::vector< Mesh >> meshes);
    virtual void update(double dt);
};

#endif // MYPHYSICENGINE_H
