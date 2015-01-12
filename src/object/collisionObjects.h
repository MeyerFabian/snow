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
        delete pPositions;
        delete pVelocities;
    }

    std::shared_ptr<std::vector<Collider> > const colliders = std::make_shared<std::vector<Collider> >();

    void initVBO();
    void updateVBOBuffer();
    void render();

    void initSSBO();
    void updateSSBOBuffer();
    void debug();
    Vector4f* pPositions= nullptr ;
    Vector4i* pVelocities= nullptr ;
    Matrix4f* pForcesE= nullptr ;
    Matrix4f* pForcesP= nullptr ;
    Vector4i* pVelocitiesn = nullptr;
private:



    GLuint gvelBn0;
    GLuint gvelBn1;
    GLuint posB;
    GLuint velB;
    GLuint velBn;
    GLuint FEpB;
    GLuint FPpB;
};
#endif // COLLISIONOBJECTS_H
