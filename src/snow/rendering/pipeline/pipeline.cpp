#include "pipeline.hpp"
#include <iostream>
const Matrix4f* pipeline::getMVP() {
  Matrix4f Cam, CamPos, Persp;

  Cam.LoadCameraMatrix(this->m_camera.xAxis, this->m_camera.yAxis,
                       this->m_camera.zAxis);
  CamPos.LoadCameraPosMatrix(this->m_camera.pos);
  Persp.LoadPerspMatrix(this->m_persp);
  this->MVP = Persp * Cam * CamPos * (*getModelMatrix());
  return &this->MVP;
}

const Matrix4f* pipeline::getModelMatrix() {
  Matrix4f Scale, Trans, Rot;
  Scale.LoadScale(this->scale);
  Trans.LoadPosition(this->position);
  Rot.LoadRotation(this->rotation);
  this->modelMatrix = Trans * Rot * Scale;

  return &this->modelMatrix;
}
void pipeline::update(const int key, const float stepsize) {
  this->m_camera.update(key, stepsize);
}
void pipeline::update(const double xpos, const double ypos) {
  this->m_camera.update(xpos, ypos);
}

void pipeline::setCamera(const float pos_x, const float pos_y,
                         const float pos_z, const float lookAt_x,
                         const float lookAt_y, const float lookAt_z,
                         const float up_x, const float up_y, const float up_z) {
  this->m_camera.setCamera(pos_x, pos_y, pos_z, lookAt_x, lookAt_y, lookAt_z,
                           up_x, up_y, up_z);
}
Vector3f pipeline::getCameraPos() { return m_camera.pos; }
