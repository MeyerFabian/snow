#include "pipeline.hpp"
#include <glm/gtx/transform.hpp>
#include <iostream>
const glm::mat4& pipeline::getMVP(float width, float height) {
  glm::mat4 Cam = m_camera.getViewMatrix();
  glm::mat4 Persp = m_camera.getProjectionMatrix(width, height);
  this->MVP = Persp * Cam * (getModelMatrix());
  return this->MVP;
}

const glm::mat4& pipeline::getModelMatrix() {
  auto Scale = glm::scale(this->scale);
  auto Trans = glm::translate(this->position);
  auto Rot = glm::rotate(glm::radians(rotation.x), glm::vec3(1, 0, 0)) *
             glm::rotate(glm::radians(rotation.y), glm::vec3(0, 1, 0)) *
             glm::rotate(glm::radians(rotation.z), glm::vec3(0, 0, 1));
  this->modelMatrix = Trans * Rot * Scale;

  return this->modelMatrix;
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
glm::vec3 pipeline::getCameraPos() { return m_camera.pos; }

