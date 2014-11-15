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
    Vector4f* pPositions= nullptr ;
    Vector4f* pVelocities= nullptr ;

private:



    GLuint VB;
    GLuint posB;
    GLuint massB;
    GLuint velB;
};
#endif // PARTICLESYSTEM_H
