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
    //glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vector3f) * (pPositions)->size(), pPositions->data());
}

void Grid::initSSBO(){
    glGenBuffers(1,&massB);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, massB);
    glBufferData(GL_SHADER_STORAGE_BUFFER,sizeof(float) * (gridPoints)->size(), NULL, GL_STREAM_DRAW);
    glUnmapBuffer ( GL_SHADER_STORAGE_BUFFER ) ;
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, GRID_MASS_BUFFER , massB);
    std::cout << "GridMassBufferSize: "<< sizeof(float) * (gridPoints)->size()/1024 << " KB" <<std::endl;
    glGenBuffers(1,&velB);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, velB);
    glBufferData(GL_SHADER_STORAGE_BUFFER,sizeof(Vector4f) * (gridPoints)->size(), NULL, GL_STATIC_DRAW);
    glUnmapBuffer ( GL_SHADER_STORAGE_BUFFER ) ;
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, GRID_VEL_BUFFER , velB);
    std::cout << "GridVelocityBufferSize: "<< sizeof(Vector4f) * (gridPoints)->size()/1024 << " KB" <<std::endl;
}
