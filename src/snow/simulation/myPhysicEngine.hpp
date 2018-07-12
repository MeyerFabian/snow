#ifndef MYPHYSICENGINE_H
#define MYPHYSICENGINE_H
#include "../simulation/physicEngine.hpp"
#include "timeUpdate.hpp"
#include "timeUpdate/explicittimeupdate.hpp"
#include "../object/grid.hpp"


class myPhysicEngine : public physicEngine
{
public:
    myPhysicEngine( std::shared_ptr<TimeUpdate> const update):physicEngine( update){

    }

    virtual bool init();
    virtual void update(double dt);
};

#endif // MYPHYSICENGINE_H
