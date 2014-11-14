#ifndef GRID_H
#define GRID_H
#define GLEW_STATIC
#include "glew.h"

#include <vector>
#include "gridPoint.h"
#include <memory>
#include <glm.hpp>
#include <iostream>
class Grid{
public:
    Grid()= default;

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

    GLuint VB;

    std::shared_ptr<std::vector<Vector3f> > const pPositions = std::make_shared< std::vector <Vector3f> >();

    inline glm::ivec3 getIJK(int index){
        int temp = index%(dimx*dimy);
        return glm::ivec3(temp%dimx, temp/dimx, index/(dimx*dimy));
    }

    void initVBO(int res_x,int res_y,int res_z);
    void updateVBOBuffer();
    void render();
private:
    void computeParticlePositions (int resolutionx, int resolutiony,int resolutionz){
        for(int i = 0; i< dimx ; i+=resolutionx){
            for(int j = 0; j< dimy ; j+=resolutiony){
                for(int k = 0; k< dimz ; k+=resolutionz){

                    pPositions->push_back(Vector3f(x_off + h*(float)i,y_off + h*(float)j,z_off + h*(float)k));
                }
            }

        }
    }

};



#endif // GRID_H
