#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H
#define GLEW_STATIC
#include "glew.h"

#include "particle.h"
#include <memory>
#include <vector>
#include "../math3d.h"
#include <fstream>
#include <string>
#include "../defines.h"
class ParticleSystem{
public:

    ParticleSystem() = default;
    ~ParticleSystem(){
        delete pPositions;
        delete pVelocities;
        delete pForcesE;
        delete pForcesP;
        delete pVelocitiesn;
        delete pDeltaV0;
        delete pDeltaV1;
        delete pDeltaV2;
    }

    std::shared_ptr<std::vector<Particle> > const particles = std::make_shared<std::vector<Particle> >();

    void initVBO();
    void updateVBOBuffer();
    void render();
    void initParticlesFromFile(const std::string& filename);
    void initSSBO();
    void updateSSBOBuffer();
    void debug();
    Vector4f* pPositions= nullptr ;
    Vector4i* pVelocities= nullptr ;
    Matrix4f* pForcesE= nullptr ;
    Matrix4f* pForcesP= nullptr ;
    Vector4i* pVelocitiesn = nullptr;
    Vector4i* pDeltaV0 = nullptr;
    Vector4i* pDeltaV1 = nullptr;
    Vector4i* pDeltaV2 = nullptr;
private:



    GLuint posB;
    GLuint velB;
    GLuint velBn;
    GLuint deltaV0;
    GLuint deltaV1;
    GLuint deltaV2;
    GLuint FEpB;
    GLuint FPpB;
};
#endif // PARTICLESYSTEM_H
