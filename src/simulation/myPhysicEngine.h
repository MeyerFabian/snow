#ifndef MYPHYSICENGINE_H
#define MYPHYSICENGINE_H
#include "../physicEngine.h"
#include "timeUpdate.h"
#include "timeUpdate/explicittimeupdate.h"
#include "../object/grid.h"

class myPhysicEngine : public physicEngine
{
public:
    myPhysicEngine( std::shared_ptr<TimeUpdate> const update):physicEngine( update){

    }

    virtual bool init();
    virtual void update(double dt);
};

#endif // MYPHYSICENGINE_H
