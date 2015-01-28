#include "grid.h"
void Grid::initVBO() {
    /*
    glGenBuffers(1,&VB);
    glBindBuffer(GL_ARRAY_BUFFER, posB);
    glBufferData(GL_ARRAY_BUFFER,sizeof(Vector3f) * (gridPoints)->size(), NULL, GL_STREAM_DRAW);
*/
}

void Grid::render(){


    glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, posB);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

        glDrawArrays(GL_POINTS,0,(gridPoints)->size());

    glDisableVertexAttribArray(0);
}

void Grid::renderBorders(){

    glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, borderVB);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,borderIB);
        glDrawElements(GL_LINE_STRIP,(iGridBorders).size(),GL_UNSIGNED_INT,0);



    glDisableVertexAttribArray(0);

}

void Grid::resetSSBOBuffer(){
    //glBindBuffer(GL_SHADER_STORAGE_BUFFER,posB);
  //glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * (pPositions)->size(), NULL, GL_STREAM_DRAW);
   // glBufferSubData(GL_SHADER_STORAGE_BUFFER, 12, sizeof(Vector4f) * (gridPoints)->size(),0);
}
void Grid::debug(){
    std::cout << "Grid"<< std::endl;
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, posB);
    Vector4f* p = (Vector4f*) (glMapBufferRange(GL_SHADER_STORAGE_BUFFER,0,sizeof(Vector4f)* (gridPoints)->size(), GL_MAP_READ_BIT));
    std::cout << "xi: ";p[9+4*201+9*201*201].print();
    glUnmapBuffer ( GL_SHADER_STORAGE_BUFFER);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, velB);
    Vector4i* c = (Vector4i*) (glMapBufferRange(GL_SHADER_STORAGE_BUFFER,0,2*sizeof(Vector4i)* (gridPoints)->size(), GL_MAP_READ_BIT));
    std::cout << "vi: ";c[2*(9+4*201+9*201*201)].print();
    std::cout << "fi: ";c[2*(9+4*201+9*201*201)+1].print();
    glUnmapBuffer ( GL_SHADER_STORAGE_BUFFER);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, velBn);
    Vector4f* v = (Vector4f*) (glMapBufferRange(GL_SHADER_STORAGE_BUFFER,0,sizeof(Vector4f)* (gridPoints)->size(), GL_MAP_READ_BIT));
    std::cout << "gvn: ";v[(9+4*201+9*201*201)].print();
    glUnmapBuffer ( GL_SHADER_STORAGE_BUFFER);


}

void Grid::initSSBO(){

    glGenBuffers(1,&borderVB);
    glBindBuffer(GL_ARRAY_BUFFER, borderVB);
    glBufferData(GL_ARRAY_BUFFER,sizeof(Vector4f) * gridBorders.size(), &gridBorders[0], GL_STATIC_DRAW);

    glGenBuffers(1,&borderIB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,borderIB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(unsigned int) * iGridBorders.size(),&iGridBorders[0], GL_STATIC_DRAW);


    //approacing zero driver overhead
    GLbitfield mapFlags = GL_MAP_WRITE_BIT| GL_MAP_INVALIDATE_BUFFER_BIT;


    glGenBuffers(1,&posB);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, posB);
    glBufferData(GL_SHADER_STORAGE_BUFFER,sizeof(Vector4f) * (gridPoints)->size(), NULL, GL_STATIC_DRAW);
    pPositions =(Vector4f*) (glMapBufferRange(GL_SHADER_STORAGE_BUFFER,0,sizeof(Vector4f) * (gridPoints)->size(),mapFlags));
    int index=0;

    for(int k = 0; k< dimz ; k++){
        for(int j = 0; j< dimy ; j++){
            for(int i = 0; i< dimx ; i++){
                pPositions[index] = Vector4f(x_off + h*(float)i,y_off + h*(float)j,z_off + h*(float)k,0);

                //pPositions[index].print();
                index++;
            }
        }
    }

    glUnmapBuffer ( GL_SHADER_STORAGE_BUFFER);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER,GRID_POS_BUFFER, posB);

    std::cout << "Amount of gridPoints: " << gridPoints->size()<<std::endl;


    std::cout << "GridPositionBufferSize: "<< sizeof(Vector4f) * (gridPoints)->size()/1024 << " KB" <<std::endl;

    glGenBuffers(1,&velB);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, velB);
    glBufferData(GL_SHADER_STORAGE_BUFFER,2*sizeof(Vector4i) * (gridPoints)->size(), NULL, GL_STATIC_DRAW);
    glUnmapBuffer ( GL_SHADER_STORAGE_BUFFER ) ;
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, GRID_VEL_BUFFER , velB);
    std::cout << "GridVelocityBufferSize: "<< 2*sizeof(Vector4i) * (gridPoints)->size()/1024 << " KB" <<std::endl;

    glGenBuffers(1,&velBn);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, velBn);
    glBufferData(GL_SHADER_STORAGE_BUFFER,sizeof(Vector4i) * (gridPoints)->size(), NULL, GL_STATIC_DRAW);
    glUnmapBuffer ( GL_SHADER_STORAGE_BUFFER ) ;
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, GRID_VEL_N_BUFFER , velBn);
    std::cout << "GridVelocityBufferSize: "<< sizeof(Vector4i) * (gridPoints)->size()/1024 << " KB" <<std::endl;


}
