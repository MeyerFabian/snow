#ifndef overGrid_H
#define overGrid_H
#include "../../technique.h"
#include "../../defines.h"
class OverGrid :public Technique
{
public:
    OverGrid()=default;
    bool init(string cs);
void setDt(const float dt);
void setCritComp();
void setCritStretch();

private:
    GLuint dt;
    GLuint critComp;
    GLuint critStretch;
};

#endif // resetGrid_H
