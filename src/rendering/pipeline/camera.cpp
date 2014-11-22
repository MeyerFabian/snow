#include "camera.h"
#include <iostream>
#include <GLFW/glfw3.h>
Camera::Camera(){

}

void Camera::update(const int key,const  float stepsize){

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

void Camera::setCamera(const float pos_x,const  float pos_y,const  float pos_z,const  float lookAt_x,const float lookAt_y,const float lookAt_z,const  float up_x,const float up_y,const float up_z){
    this->pos = Vector3f(pos_x,pos_y,pos_z);
    Vector3f lookAt = Vector3f(lookAt_x,lookAt_y,lookAt_z);
    Vector3f up = Vector3f(up_x,up_y,up_z);
    this->zAxis = lookAt-this->pos;
    this->zAxis.normalize();

    up.normalize();
    this->xAxis = this->zAxis.cross(up);
    this->yAxis = this->xAxis.cross(this->zAxis);


}
