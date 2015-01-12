#ifndef COLLIDER_H
#define COLLIDER_H
#include "../math3d.h"
struct Collider{

    Collider(Vector4f p = Vector4f(0.0f,0.0f,0.0f,0.0f),
            Vector3f v = Vector3f(0.0f,0.0f,0.0f),
            Vector3f n = Vector3f(0.0f,1.0f,0.0f),
            int t= 0,
            float f  = 0.1f)
    :position(p), velocity(v),normal(n), type(t),friction(f){}


    Vector4f position;
    Vector3f velocity;
    Vector3f normal;
    float friction;
    int type;

};

#endif // COLLIDER_H
