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

    ParticleSystem() = default;
    std::shared_ptr<std::vector<Particle> > const particles = std::make_shared<std::vector<Particle> >();

    void initVBO();
    void updateVBOBuffer();
    void render();

    void initSSBO();
    void updateSSBOBuffer();
    std::shared_ptr<std::vector<Vector3f> > const pPositions = std::make_shared< std::vector <Vector3f> >();
    std::shared_ptr<std::vector<float> > const pMasses = std::make_shared< std::vector <float> >();

private:


    void computeParticlePositions (){
        for(auto& particle : (*particles)){
            pPositions->push_back(particle.position);
        }
    }
    void computeParticleMasses(){
        for(auto& particle: (*particles)){
            pMasses->push_back(particle.mass);
        }
    }

    GLuint VB;
    GLuint massB;
};
#endif // PARTICLESYSTEM_H
