#include "camera.h"
#include <iostream>
#include <GLFW/glfw3.h>
Camera::Camera(){

}

void Camera::update(int key, float stepsize){

    if(key == GLFW_KEY_RIGHT){
        this->pos = this->pos - (this->xAxis * stepsize);
    }

    else if(key == GLFW_KEY_LEFT){
        this->pos = this->pos + (this->xAxis *stepsize);
    }

    else if(key == GLFW_KEY_DOWN){

        this->pos = this->pos - (this->zAxis *stepsize);
    }

    else if(key == GLFW_KEY_UP){

        this->pos = this->pos + (this->zAxis *stepsize);
    }
    else{
        std::cerr << key << " was not recognised and thrown away." <<std::endl;
    }
}

void Camera::setCamera(float pos_x, float pos_y, float pos_z, float lookAt_x,float lookAt_y,float lookAt_z, float up_x,float up_y,float up_z){
    this->pos = Vector3f(pos_x,pos_y,pos_z);
    Vector3f lookAt = Vector3f(lookAt_x,lookAt_y,lookAt_z);
    Vector3f up = Vector3f(up_x,up_y,up_z);
    this->zAxis = lookAt-this->pos;
    this->zAxis.normalize();

    up.normalize();
    this->xAxis = this->zAxis.cross(up);
    this->yAxis = this->xAxis.cross(this->zAxis);


}
