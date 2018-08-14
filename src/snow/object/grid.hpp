#ifndef GRID_H
#define GRID_H
#define GLEW_STATIC
#include <GL/glew.h>

#include <iostream>
#include <memory>
#include <vector>
#include "../utils/defines.hpp"
#include "gridPoint.hpp"
#include "math3d.hpp"
class Grid {
 public:
  Grid() = default;
  ~Grid() { delete pPositions; }

  std::vector<GridPoint> gridPoints;
  std::vector<Vector4f> gridBorders;
  std::vector<unsigned int> iGridBorders;
  Grid(int x, int y, int z, float spacing, float pos_x, float pos_y,
       float pos_z)
      : dimx(x),
        dimy(y),
        dimz(z),
        h(spacing),
        x_off(pos_x),
        y_off(pos_y),
        z_off(pos_z) {
    for (int i = 0; i < dimx * dimy * dimz; i++) {
      gridPoints.push_back(GridPoint());
    }
    float above =
        spacing * ((float)y) - (float(GRID_COLLISION_PLANE_OFFSET)) * spacing;
    float right =
        spacing * ((float)x) - (float(GRID_COLLISION_PLANE_OFFSET)) * spacing;
    float before =
        spacing * ((float)z) - (float(GRID_COLLISION_PLANE_OFFSET)) * spacing;
    float behind = (float(GRID_COLLISION_PLANE_OFFSET)) * spacing;
    float left = (float(GRID_COLLISION_PLANE_OFFSET)) * spacing;
    float below = (float(GRID_COLLISION_PLANE_OFFSET)) * spacing;
    gridBorders.push_back(
        Vector4f(pos_x + left, pos_y + below, pos_z + behind, 1.0f));
    gridBorders.push_back(
        Vector4f(pos_x + left, pos_y + above, pos_z + behind, 1.0f));
    gridBorders.push_back(
        Vector4f(pos_x + right, pos_y + above, pos_z + behind, 1.0f));
    gridBorders.push_back(
        Vector4f(pos_x + right, pos_y + below, pos_z + behind, 1.0f));
    gridBorders.push_back(
        Vector4f(pos_x + left, pos_y + below, pos_z + before, 1.0f));
    gridBorders.push_back(
        Vector4f(pos_x + left, pos_y + above, pos_z + before, 1.0f));
    gridBorders.push_back(
        Vector4f(pos_x + right, pos_y + above, pos_z + before, 1.0f));
    gridBorders.push_back(
        Vector4f(pos_x + right, pos_y + below, pos_z + before, 1.0f));
    // gridBorders.at(0).print();
    iGridBorders.push_back(0);
    iGridBorders.push_back(1);
    iGridBorders.push_back(5);
    iGridBorders.push_back(4);
    iGridBorders.push_back(0);
    iGridBorders.push_back(3);
    iGridBorders.push_back(7);
    iGridBorders.push_back(4);
    iGridBorders.push_back(5);
    iGridBorders.push_back(6);
    iGridBorders.push_back(7);
    iGridBorders.push_back(3);
    iGridBorders.push_back(2);
    iGridBorders.push_back(6);
    iGridBorders.push_back(5);
    iGridBorders.push_back(1);
    iGridBorders.push_back(2);
    iGridBorders.push_back(3);
    iGridBorders.push_back(0);
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
  GLuint forceB;
  GLuint velBn;
  GLuint borderVB;
  GLuint borderIB;

  Vector4f* pPositions;

  void initVBO();
  void resetSSBOBuffer();
  void render();
  void renderBorders();
  void debug();
  void initSSBO();

 private:
};

#endif  // GRID_H

