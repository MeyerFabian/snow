#ifndef PARTICLE_H
#define PARTICLE_H
#include "../math3d.h"
struct Particle
{
    Particle(Vector3f r = Vector3f(0.0f,0.0f,0.0f),
             Vector3f v = Vector3f(0.0f,0.0f,0.0f),
             float m =6.25f, float V= 1e-9,
             Matrix4f Fe = Matrix4f(1.0f),
             Matrix4f Fp = Matrix4f(1.0f))
        : position(r), velocity(v), mass(m), volume(V) , forceElastic(Fe), forcePlastic(Fp){
    }

    Vector3f position;
    Vector3f velocity;

    float mass;
    float volume;

    Matrix4f forceElastic;
    Matrix4f forcePlastic;
};

#endif // PARTICLE_H
