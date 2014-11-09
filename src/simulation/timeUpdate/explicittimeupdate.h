#ifndef EXPLICITTIMEUPDATE_H
#define EXPLICITTIMEUPDATE_H
#include "../timeUpdate.h"
class ExplicitTimeUpdate : public TimeUpdate
{
public:
    ExplicitTimeUpdate();
    void update(double dt);
};

#endif // EXPLICITTIMEUPDATE_H
