#ifndef SEMIIMPLICITTIMEUPDATE_H
#define SEMIIMPLICITTIMEUPDATE_H
#include "../timeUpdate.h"
class SemiImplicitTimeUpdate :public TimeUpdate
{
public:
    SemiImplicitTimeUpdate();
    void update(double dt);
};

#endif // SEMIIMPLICITTIMEUPDATE_H
