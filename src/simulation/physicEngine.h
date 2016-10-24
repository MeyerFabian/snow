#ifndef PHYSICENGINE_H
#define PHYSICENGINE_H
#include <memory>
#include <vector>
#include "timeUpdate.h"

class physicEngine{
    public :
    physicEngine( std::shared_ptr<TimeUpdate> const update):  integration(update){

    }

    virtual bool init() = 0;
    virtual void update(double dt) = 0;


    std::shared_ptr<TimeUpdate> const integration;
};

#endif // PHYSICENGINE_H
