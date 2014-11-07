#ifndef MYPHYSICENGINE_H
#define MYPHYSICENGINE_H
#include "../physicEngine.h"
class myPhysicEngine : public physicEngine
{
public:
    myPhysicEngine() = default;
    virtual bool init();
    virtual void update( std::shared_ptr<std::vector< Mesh >> meshes ,double dt);
};

#endif // MYPHYSICENGINE_H
