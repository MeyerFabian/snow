#include "collisionObjects.h"
#include <iostream>

/**
 * STUB
 */


void CollisionObjects::debug(){

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, posB);
    std::cout << "pos"<<std::endl;
    Vector4f* p = (Vector4f*) (glMapBufferRange(GL_SHADER_STORAGE_BUFFER,0,sizeof(Vector4f)* (colliders)->size(), GL_MAP_READ_BIT));
    p[0].print();
    p[1].print();
    p[2].print();
    p[3].print();
    p[4].print();
    p[5].print();
    p[6].print();
    glUnmapBuffer ( GL_SHADER_STORAGE_BUFFER);
}

void CollisionObjects::initSSBO(){

    glGenBuffers(1,&posB);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, posB);
    glBufferData(GL_SHADER_STORAGE_BUFFER,sizeof(Vector4f) * (colliders)->size(), NULL, GL_STREAM_DRAW);
    cPositions =(Vector4f*) (glMapBufferRange(GL_SHADER_STORAGE_BUFFER,0,sizeof(Vector4f) * (colliders)->size(), GL_MAP_WRITE_BIT |  GL_MAP_INVALIDATE_BUFFER_BIT));
    for(int i = 0; i<colliders->size();i++){
        cPositions[i] = colliders->at(i).mesh->getPosition();
    }
    glUnmapBuffer ( GL_SHADER_STORAGE_BUFFER);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, COLLIDER_POS_BUFFER , posB);

    glGenBuffers(1,&velB);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, velB);
    glBufferData(GL_SHADER_STORAGE_BUFFER,sizeof(Vector4f) * (colliders)->size(), NULL, GL_STATIC_DRAW);
    cVelocities =(Vector4f*) (glMapBufferRange(GL_SHADER_STORAGE_BUFFER,0,sizeof(Vector4f) * (colliders)->size(),GL_MAP_WRITE_BIT |  GL_MAP_INVALIDATE_BUFFER_BIT));
    for(int i = 0; i<colliders->size();i++){
        cVelocities[i] = colliders->at(i).velocity;
    }
    glUnmapBuffer ( GL_SHADER_STORAGE_BUFFER);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, COLLIDER_VEL_BUFFER , velB);

    glGenBuffers(1,&norB);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, norB);
    glBufferData(GL_SHADER_STORAGE_BUFFER,sizeof(Vector4f) * (colliders)->size(), NULL, GL_STATIC_DRAW);
    cNormals =(Vector4f*) (glMapBufferRange(GL_SHADER_STORAGE_BUFFER,0,sizeof(Vector4f) * (colliders)->size(),GL_MAP_WRITE_BIT |  GL_MAP_INVALIDATE_BUFFER_BIT));
    for(int i = 0; i<colliders->size();i++){
        cNormals[i] = colliders->at(i).normal;
    }
    glUnmapBuffer ( GL_SHADER_STORAGE_BUFFER);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, COLLIDER_NOR_BUFFER , norB);

    glGenBuffers(1,&typeB);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, typeB);
    glBufferData(GL_SHADER_STORAGE_BUFFER,sizeof(int) * (colliders)->size(), NULL, GL_STATIC_DRAW);
    cType =(int*) (glMapBufferRange(GL_SHADER_STORAGE_BUFFER,0,sizeof(int) * (colliders)->size(),GL_MAP_WRITE_BIT |  GL_MAP_INVALIDATE_BUFFER_BIT));
    for(int i = 0; i<colliders->size();i++){
        cType[i] = colliders->at(i).type;
    }
    glUnmapBuffer ( GL_SHADER_STORAGE_BUFFER);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, COLLIDER_TYPE_BUFFER , typeB);

    glGenBuffers(1,&fricB);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, fricB);
    glBufferData(GL_SHADER_STORAGE_BUFFER,sizeof(float) * (colliders)->size(), NULL, GL_STATIC_DRAW);
    cFriction =(float*) (glMapBufferRange(GL_SHADER_STORAGE_BUFFER,0,sizeof(float) * (colliders)->size(),GL_MAP_WRITE_BIT |  GL_MAP_INVALIDATE_BUFFER_BIT));
    for(int i = 0; i<colliders->size();i++){
        cFriction[i] = colliders->at(i).friction;
    }
    glUnmapBuffer ( GL_SHADER_STORAGE_BUFFER);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, COLLIDER_FRIC_BUFFER , fricB);


}
void CollisionObjects::updateRenderBuffer(float dt){

    for(int i ; i< colliders->size();i++){

    Vector3f position = colliders->at(i).mesh->getPosition();
    Vector3f velocity = colliders->at(i).velocity.xyz();
    Vector3f uPos(position+velocity*dt);

    colliders->at(i).mesh->setPosition(uPos);
    }

}
