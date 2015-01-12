#include "collisionObjects.h"
#include <iostream>

/**
 * STUB
 */

void CollisionObjects::initVBO() {


}

void CollisionObjects::render(){
/*
    glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, posB);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
        //DrawArraysInstanced?! + DRAW ONLY GRID NODES
        glDrawArrays(GL_POINTS,0,(particles)->size());


    glDisableVertexAttribArray(0);
    */
}
void CollisionObjects::updateVBOBuffer(){


}
void CollisionObjects::debug(){








}

void CollisionObjects::initSSBO(){
    /*
    glGenBuffers(1,&posB);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, posB);
    glBufferData(GL_SHADER_STORAGE_BUFFER,sizeof(Vector4f) * (colliders)->size(), NULL, GL_STATIC_DRAW);
    pPositions =(Vector4f*) (glMapBufferRange(GL_SHADER_STORAGE_BUFFER,0,sizeof(Vector4f) * (particles)->size(),GL_MAP_WRITE_BIT |  GL_MAP_INVALIDATE_BUFFER_BIT));
    for(int i = 0; i<colliders->size();i++){
        pPositions[i] = colliders->at(i).position;
    }
    glUnmapBuffer ( GL_SHADER_STORAGE_BUFFER);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, COLLIDER_POS_BUFFER , posB);
*/

}
void CollisionObjects::updateSSBOBuffer(){
}
