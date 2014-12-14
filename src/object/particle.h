#ifndef PARTICLE_H
#define PARTICLE_H
#include "../math3d.h"
struct Particle
{
    Particle(Vector3f r = Vector3f(0.0f,0.0f,0.0f),
             Vector3f v = Vector3f(0.0f,0.0f,0.0f),
             float m = 0.0125, float V= 1e-9,
             Matrix3f Fe = Matrix3f(1.0f),
             Matrix3f Fp = Matrix3f(1.0f))
        : position(r), velocity(v), mass(m), volume(V) , forceElastic(Fe), forcePlastic(Fp){
    }

    Vector3f position;
    Vector3f velocity;

    float mass;
    float volume;

    Matrix3f forceElastic;
    Matrix3f forcePlastic;
};

#endif // PARTICLE_H
