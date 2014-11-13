#include "grid.h"
void Grid::initVBO(int res_x,int res_y,int res_z) {
    computeParticlePositions(res_x,res_y,res_z);
    glGenBuffers(1,&VB);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER,sizeof(Vector3f) * (pPositions)->size(), pPositions->data(), GL_STREAM_DRAW);
}

void Grid::render(){


    glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, VB);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        //DrawArraysInstanced?! + DRAW ONLY GRID NODES
        glDrawArrays(GL_POINTS,0,(pPositions)->size());


    glDisableVertexAttribArray(0);
}
void Grid::updateVBOBuffer(){

    glBindBuffer(GL_ARRAY_BUFFER,VB);
  //glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * (pPositions)->size(), NULL, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vector3f) * (pPositions)->size(), pPositions->data());
}
