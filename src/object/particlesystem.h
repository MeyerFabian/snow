#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H
#define GLEW_STATIC
#include "glew.h"

#include "particle.h"
#include <memory>
#include <vector>
#include "../math3d.h"

#include "../defines.h"
class ParticleSystem{
public:

    ParticleSystem() = default;
    ~ParticleSystem(){
        delete pPositions;
        delete pVelocities;
    }

    std::shared_ptr<std::vector<Particle> > const particles = std::make_shared<std::vector<Particle> >();

    void initVBO();
    void updateVBOBuffer();
    void render();

    void initSSBO();
    void updateSSBOBuffer();
    void debug();
    Vector4f* pPositions= nullptr ;
    Vector4f* pVelocities= nullptr ;
    Matrix4f* pForcesE= nullptr ;
    Matrix4f* pForcesP= nullptr ;
    Vector4f* pVelocitiesn = nullptr;
    Vector4f* pdeltaVelocitiesn0 =nullptr;
    Vector4f* pdeltaVelocitiesn1 =nullptr;
    Vector4f* pdeltaVelocitiesn2 =nullptr;
private:



    GLuint gvelBn0;
    GLuint gvelBn1;
    GLuint gvelBn2;
    GLuint posB;
    GLuint velB;
    GLuint velBn;
    GLuint FEpB;
    GLuint FPpB;
};
#endif // PARTICLESYSTEM_H
