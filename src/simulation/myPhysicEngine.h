#ifndef MYPHYSICENGINE_H
#define MYPHYSICENGINE_H
#include "../physicEngine.h"
class myPhysicEngine : public physicEngine
{
public:
    myPhysicEngine() = default;
    virtual bool init();
    virtual void update(double dt);
};

#endif // MYPHYSICENGINE_H
