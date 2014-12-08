#ifndef particleCompute_H
#define particleCompute_H
#include "../../technique.h"
#include "../../math3d.h"
#include "../../defines.h"
class ParticleCompute :public Technique
{
public:
    ParticleCompute()=default;
    bool init(string cs);
    void setGridPos(const float gridPosx,const float gridPosy,const float gridPosz);
    void setGridDim(const int gridDimx,const int gridDimy,const int gridDimz);
    void setGridSpacing (const float h);

    void setYoung();
    void setPoisson();
    void setHardening();
    void setCritComp();
    void setCritStretch();

private:
    GLuint gGridPos;
    GLuint gGridDim;
    GLuint h;

    GLuint young;
    GLuint poisson;
    GLuint hardening;
    GLuint critComp;
    GLuint critStretch;
};
#endif // particleCompute_H
