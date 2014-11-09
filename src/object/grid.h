#ifndef GRID_H
#define GRID_H
#include <vector>
#include "gridPoint.h"
#include <memory>
struct Grid{
    Grid()= default;

    std::shared_ptr<std::vector<GridPoint>> gridPoints;

    Grid(int x, int y , int z) : dimx(x),dimy(y), dimz(z){

        gridPoints = make_shared<std::vector<GridPoint>>();
        gridPoints->resize(dimx*dimy*dimz);
    }

    int dimx;
    int dimy;
    int dimz;
};
#endif // GRID_H
