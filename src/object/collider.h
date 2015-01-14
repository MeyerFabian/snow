#ifndef COLLIDER_H
#define COLLIDER_H
#include "../math3d.h"
#include "mesh.h"
struct Collider{


    /**
     * @brief Collider STUB: Will not use normals from the mesh for the beginning
     * @param mesh
     * @param v
     * @param n
     * @param f
     * @param t
     */
    Collider(std::shared_ptr<Mesh> const attachedMesh,float w=0.0f,int t= 0, Vector3f v = Vector3f(0.0f,0.0f,0.0f),Vector3f n = Vector3f(0.0f,0.0f,0.0f), float f = 0.1f)
            : mesh(attachedMesh),velocity(v,w),normal(n), friction(f), type(t){

    }

    std::shared_ptr<Mesh>  const mesh = std::make_shared<Mesh>();

    Vector4f velocity;
    Vector3f normal;
    float friction;
    int type;

};

#endif // COLLIDER_H
