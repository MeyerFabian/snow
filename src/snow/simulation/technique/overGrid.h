#ifndef overGrid_H
#define overGrid_H
#include "../../shader/technique.h"
#include "../../defines/defines.h"
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
void setnumColliders(const int numColliders);
private:
    GLuint dt;
    GLuint critComp;
    GLuint critStretch;
    GLuint collisionOffset;
    GLuint gGridDimension;
    GLuint gNumColliders;
};

#endif // resetGrid_H
