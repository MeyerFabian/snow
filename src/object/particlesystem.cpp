#include "particlesystem.h"
#include <iostream>
void ParticleSystem::initVBO() {
    computeParticlePositions();
    glGenBuffers(1,&VB);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER,sizeof(Vector3f) * (pPositions)->size(), pPositions->data(), GL_STATIC_DRAW);
}

void ParticleSystem::render(){

    glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, VB);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_POINTS,0,(pPositions)->size());


    glDisableVertexAttribArray(0);
}
