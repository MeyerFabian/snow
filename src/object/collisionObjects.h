#ifndef COLLISIONOBJECTS_H
#define COLLISIONOBJECTS_H
#define GLEW_STATIC
#include "glew.h"

#include "collider.h"
#include <memory>
#include <vector>
#include "../math3d.h"

#include "../defines.h"
class CollisionObjects{
public:

    CollisionObjects() = default;
    ~CollisionObjects(){
        delete cPositions;
        delete cVelocities;
        delete cType;
        delete cFriction;
        delete cNormals;
    }

    std::shared_ptr<std::vector<Collider> > const colliders = std::make_shared<std::vector<Collider> >();


    void initSSBO();
    void updateRenderBuffer(float dt);
    void debug();
    Vector4f* cPositions= nullptr ;
    Vector4f* cVelocities= nullptr ;
    int* cType= nullptr ;
    float* cFriction= nullptr ;
    Vector4f* cNormals= nullptr;
private:



    GLuint posB;
    GLuint velB;
    GLuint norB;
    GLuint typeB;
    GLuint fricB;
};
#endif // COLLISIONOBJECTS_H
