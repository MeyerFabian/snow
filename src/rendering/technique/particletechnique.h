#ifndef PARTICLETECHNIQUE_H
#define PARTICLETECHNIQUE_H
#include "technique.h"
#include "../../math3d.h"
class ParticleTechnique :public Technique
{
public:
    ParticleTechnique();
    bool init(string vs, string fs);
    void setWVP(const Matrix4f*);
private:
    GLuint ModelViewPersp;
};

#endif // PARTICLETECHNIQUE_H
