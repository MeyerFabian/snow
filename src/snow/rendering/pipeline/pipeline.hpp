#ifndef PIPELINE_H
#define PIPELINE_H

#include <glm/glm.hpp>
#include "camera.hpp"

class pipeline {
 public:
  const glm::mat4 &getMVP(float width, float height);
  const glm::mat4 &getModelMatrix();

  void setScale(const glm::vec3 &scl) { scale = scl; }
  void setRotation(const glm::vec3 &angle) { rotation = angle; }
  void setPosition(const glm::vec3 &pos) { position = pos; }
  glm::vec3 getCameraPos();
  void setCamera(const float pos_x, const float pos_y, const float pos_z,
                 const float lookAt_x, const float lookAt_y,
                 const float lookAt_z, const float up_x, const float up_y,
                 const float up_z);
  void update(const int key, const float stepsize);
  void update(const double xpos, const double ypos);

 private:
  glm::vec3 scale;
  glm::vec3 rotation;
  glm::vec3 position;
  glm::mat4 modelMatrix;
  glm::mat4 MVP;
  Camera m_camera;
};

#endif  // PIPELINE_H

