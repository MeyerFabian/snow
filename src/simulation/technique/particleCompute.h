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
private:
    GLuint gGridPos;
};

#endif // particleCompute_H
