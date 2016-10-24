#ifndef GRIDPOINT_H
#define GRIDPOINT_H
#include "math3d.h"
struct GridPoint{

    GridPoint(Vector3f v = Vector3f(0.0f,0.0f,0.0f),
             float m = 0,
             Matrix3f Fe = Matrix3f(1.0f),
             Matrix3f Fp = Matrix3f(1.0f))
            : velocity(v), mass(m), forceElastic(Fe), forcePlastic(Fp){}


    Vector3f velocity;

    float mass;

    Matrix3f forceElastic;
    Matrix3f forcePlastic;
};

#endif // GRIDPOINT_H
