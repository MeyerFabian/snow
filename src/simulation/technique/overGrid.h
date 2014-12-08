#ifndef overGrid_H
#define overGrid_H
#include "../../technique.h"
class OverGrid :public Technique
{
public:
    OverGrid()=default;
    bool init(string cs);
void setDt(const float dt);

private:
    GLuint dt;
};

#endif // resetGrid_H
