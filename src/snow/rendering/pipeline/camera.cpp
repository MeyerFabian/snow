#include "camera.hpp"
#include <iostream>
#include <GLFW/glfw3.h>
Camera::Camera(){

}

void Camera::update(const int key,const  float stepsize){

    if(key == GLFW_KEY_RIGHT){
        this->pos = this->pos + (this->xAxis * stepsize);
    }

    else if(key == GLFW_KEY_LEFT){
        this->pos = this->pos - (this->xAxis *stepsize);
    }

    else if(key == GLFW_KEY_DOWN){

        this->pos = this->pos - (this->zAxis *stepsize);
    }

    else if(key == GLFW_KEY_UP){

        this->pos = this->pos + (this->zAxis *stepsize);
        //pos.print();
        //zAxis.print();
    }
    else if(key ==GLFW_KEY_1){
        setCamera(-0.0695547f, 4.16257f, 6.31657f,2.5f,1.0f,2.0f,0.0f,1.0f,0.0f);
    }
    else if(key ==GLFW_KEY_2){
        setCamera(-1.5f, 5.60813f, 10.79671f,6.7719f,0.0813f,2.59671f,0.0f,1.0f,0.0f);

    }
    else if(key == GLFW_KEY_3){
        setCamera(2.2f, 4.95986f, 8.69347f,2.2f,1.0f,2.0f,0.0f,1.0f,0.0f);
    }
    else if(key==GLFW_KEY_4){

        setCamera(-0.0695547f,2.16257f, 4.31657f,1.5f,1.0f,1.0f,0.0f,1.0f,0.0f);
    }
    else{
        std::cerr << key << " was not recognised and thrown away." <<std::endl;
    }
}
 void Camera::update(const double xpos,const double ypos){
     setCamera(this->pos.x,this->pos.y,this->pos.z,
               this->pos.x+zAxis.x+xAxis.x*xpos+yAxis.x*ypos,
               this->pos.y+zAxis.y+xAxis.y*xpos+yAxis.y*ypos,
               this->pos.z+zAxis.z+xAxis.z*xpos+yAxis.z*ypos,
               0.0,1.0,0.0 );
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
