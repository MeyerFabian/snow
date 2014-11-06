#ifndef SHADOW_MAP_TECHNIQUE_H
#define SHADOW_MAP_TECHNIQUE_H
#include "technique.h"
#include "../../math3d.h"
class ShadowMapTechnique : public Technique
{
public:
    ShadowMapTechnique();
    bool init(string vs, string fs);
    void setMVP(const Matrix4f* m);
    void setTex(unsigned int texture);
private:
    GLuint gMVP;
    GLuint gSampler;
};

#endif // SHADOW_MAP_TECHNIQUE_H
