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
void setCollisionOffset();
void setGridDim(const int gridDimx,const int gridDimy,const int gridDimz);

private:
    GLuint dt;
    GLuint critComp;
    GLuint critStretch;
    GLuint collisionOffset;
    GLuint gGridDimension;
};

#endif // resetGrid_H
