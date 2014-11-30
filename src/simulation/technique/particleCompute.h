#ifndef particleCompute_H
#define particleCompute_H
#include "../../technique.h"
#include "../../math3d.h"
class ParticleCompute :public Technique
{
public:
    ParticleCompute()=default;
    bool init(string cs);
    void setGridPos(const float gridPosx,const float gridPosy,const float gridPosz);
    void setGridDim(const int gridDimx,const int gridDimy,const int gridDimz);
    void setGridSpacing (const float h);
private:
    GLuint gGridPos;
    GLuint gGridDim;
    GLuint h;
};

#endif // particleCompute_H
