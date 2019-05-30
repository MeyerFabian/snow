#ifndef CAMERA_H
#define CAMERA_H
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <iostream>
class Camera {
 public:
  glm::mat4 getViewMatrix() const;
  glm::mat4 getProjectionMatrix(float width, float height) const;
  void setCamera(const float pos_x, const float pos_y, const float pos_z,
                 const float lookAt_x, const float lookAt_y,
                 const float lookAt_z, const float up_x, const float up_y,
                 const float up_z);
  void update(const int key, const float stepsize);
  void update(const double xpos, const double ypos);
  glm::vec3 xAxis;
  glm::vec3 zAxis;
  glm::vec3 up;
  glm::vec3 pos;
};
#endif  // CAMERA_H

