#ifndef CAMERA_H
#define CAMERA_H
#include "../math3d.h"
class Camera
{
public:
    Camera();
    void setCamera(float pos_x, float pos_y, float pos_z, float lookAt_x,float lookAt_y,float lookAt_z, float up_x,float up_y,float up_z);
    void update(int key, float stepsize);
    Vector3f xAxis;
    Vector3f yAxis;
    Vector3f zAxis;
    Vector3f pos;

};
#endif // CAMERA_H
