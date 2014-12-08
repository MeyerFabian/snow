#ifndef GRID_H
#define GRID_H
#define GLEW_STATIC
#include "glew.h"

#include <vector>
#include "gridPoint.h"
#include <memory>
#include <glm.hpp>
#include <iostream>

#include "../defines.h"
class Grid{
public:
    Grid()= default;
    ~Grid(){
        delete pPositions;
    }

    std::shared_ptr<std::vector<GridPoint>> const gridPoints = std::make_shared<std::vector<GridPoint>>();

    Grid(int x, int y , int z, float spacing, float pos_x, float pos_y, float pos_z)
        : dimx(x),dimy(y), dimz(z), h(spacing), x_off(pos_x), y_off(pos_y), z_off(pos_z){

         for(int i = 0; i< dimx*dimy*dimz ; i++){
            gridPoints->push_back(GridPoint());
         }

    }

    int dimx;
    int dimy;
    int dimz;
    float h;
    float x_off;
    float y_off;
    float z_off;

    GLuint posB;
    GLuint velB;
    GLuint velBn;
    GLuint forceB;

    Vector4f* pPositions;

    inline glm::ivec3 getIJK(int index){
        int temp = index%(dimx*dimy);
        return glm::ivec3(temp%dimx, temp/dimx, index/(dimx*dimy));
    }

    void initVBO();
    void resetSSBOBuffer();
    void render();

    void initSSBO();
private:



};



#endif // GRID_H
