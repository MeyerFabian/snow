#include "particlesystem.h"
#include <iostream>
void ParticleSystem::initVBO() {
    computeParticlePositions();
    glGenBuffers(1,&VB);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER,sizeof(Vector3f) * (pPositions)->size(), pPositions->data(), GL_STREAM_DRAW);
}

void ParticleSystem::render(){


    glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, VB);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        //DrawArraysInstanced?! + DRAW ONLY GRID NODES
        glDrawArrays(GL_POINTS,0,(pPositions)->size());


    glDisableVertexAttribArray(0);
}
void ParticleSystem::updateVBOBuffer(){

    glBindBuffer(GL_ARRAY_BUFFER,VB);
  //glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * (pPositions)->size(), NULL, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vector3f) * (pPositions)->size(), pPositions->data());
}

void ParticleSystem::initSSBO(){
    computeParticleMasses();
    glGenBuffers(1,&massB);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, massB);
    glBufferData(GL_SHADER_STORAGE_BUFFER,sizeof(float) * (pMasses)->size(), pMasses->data(), GL_STREAM_DRAW);

}
void ParticleSystem::updateSSBOBuffer(){

    glBindBuffer(GL_SHADER_STORAGE_BUFFER,massB);
  //glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * (pPositions)->size(), NULL, GL_STREAM_DRAW);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(Vector3f) * (pMasses)->size(), pMasses->data());
}
