#ifndef resetGrid_H
#define resetGrid_H
#include "../../technique.h"
class ResetGrid :public Technique
{
public:
    ResetGrid()=default;
    bool init(string cs);
private:
};

#endif // resetGrid_H
