#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H
#define GLEW_STATIC
#include "glew.h"

#include "particle.h"
#include <memory>
#include <vector>
#include "../math3d.h"


class ParticleSystem{
public:

    ParticleSystem()= default;
    std::shared_ptr<std::vector<Particle> > const particles = std::make_shared<std::vector<Particle> >();

    void initVBO();
    void render();
    void updateBuffers();
private:
    std::shared_ptr<std::vector<Vector3f> > const pPositions = std::make_shared<std::vector<Vector3f> >();

    void computeParticlePositions (){
        for(auto& particle : (*particles)){
            pPositions->push_back(particle.position);
        }
    }

    GLuint VB;
};
#endif // PARTICLESYSTEM_H
