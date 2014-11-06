#include "pipeline.h"
#include <iostream>
const Matrix4f* pipeline::getMVP(){
    Matrix4f Cam,CamPos ,Persp;

    Cam.LoadCameraMatrix(this->m_camera.xAxis,this->m_camera.yAxis,this->m_camera.zAxis);
    CamPos.LoadCameraPosMatrix( this->m_camera.pos);
    Persp.LoadPerspMatrix(this->m_persp);
    this->MVP = Persp* Cam* CamPos* (*getModelMatrix());
    return &this->MVP;
}

const Matrix4f* pipeline::getModelMatrix(){

    Matrix4f Scale, Trans, Rot;
    Scale.LoadScale(this->scale);
    Trans.LoadPosition(this->position);
    Rot.LoadRotation(this->rotation);
    this->modelMatrix = Trans * Rot * Scale;

    return &this->modelMatrix;
}
void pipeline::update(int key, float stepsize){
    this->m_camera.update(key,stepsize);

}
void pipeline::setCamera(float pos_x, float pos_y, float pos_z, float lookAt_x, float lookAt_y, float lookAt_z, float up_x, float up_y, float up_z){
    this->m_camera.setCamera(pos_x,pos_y,pos_z,lookAt_x,lookAt_y,lookAt_z,up_x,up_y,up_z);
}
Vector3f pipeline::getCameraPos(){
    return m_camera.pos;
}
