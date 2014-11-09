#ifndef GRIDPOINT_H
#define GRIDPOINT_H
#include "../math3d.h"
struct GridPoint{

    GridPoint(Vector3f r = Vector3f(0.0f,0.0f,0.0f),
             Vector3f v = Vector3f(0.0f,0.0f,0.0f),
             float m = 0,
             Matrix3f Fe = Matrix3f(1.0f),
             Matrix3f Fp = Matrix3f(1.0f))
            : position(r), velocity(v), mass(m), forceElastic(Fe), forcePlastic(Fp){}


    Vector3f position;
    Vector3f velocity;

    float mass;

    Matrix3f forceElastic;
    Matrix3f forcePlastic;
};

#endif // GRIDPOINT_H
