#ifndef CAMERA_H
#define CAMERA_H
#include "math3d.h"
class Camera
{
public:
    Camera();
    void setCamera(const float pos_x, const float pos_y, const float pos_z, const float lookAt_x,const float lookAt_y,const float lookAt_z, const float up_x,const float up_y,const float up_z);
    void update(const int key, const float stepsize);
    void update(const double xpos,const double ypos);
    Vector3f xAxis;
    Vector3f yAxis;
    Vector3f zAxis;
    Vector3f pos;

};
#endif // CAMERA_H
