#ifndef particleCompute_H
#define particleCompute_H
#include "../../technique.h"
#include "../../math3d.h"
class ParticleCompute :public Technique
{
public:
    ParticleCompute();
    bool init(string cs);
private:
};

#endif // particleCompute_H
