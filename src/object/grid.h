#ifndef GRID_H
#define GRID_H
#include <vector>
#include "gridPoint.h"
#include <memory>
struct Grid{
    Grid()= default;

    std::shared_ptr<std::vector<GridPoint>> const gridPoints = make_shared<std::vector<GridPoint>>();

    Grid(int x, int y , int z, double spacing) : dimx(x),dimy(y), dimz(z), h(spacing){
        gridPoints->resize(dimx*dimy*dimz);
    }

    int dimx;
    int dimy;
    int dimz;
    double h;
};
#endif // GRID_H
