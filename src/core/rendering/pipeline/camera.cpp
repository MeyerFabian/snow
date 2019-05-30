#include "camera.hpp"

glm::mat4 Camera::getViewMatrix() const {
  return glm::lookAt(pos, pos + zAxis, up);
}

glm::mat4 Camera::getProjectionMatrix(float width, float height) const {
  return glm::perspective(glm::radians(35.0f), width / height, 0.1f, 400.f);
}

void Camera::update(const int key, const float stepsize) {
  if (key == GLFW_KEY_RIGHT) {
    this->pos = this->pos + (this->xAxis * stepsize);
  }

  else if (key == GLFW_KEY_LEFT) {
    this->pos = this->pos - (this->xAxis * stepsize);
  }

  else if (key == GLFW_KEY_DOWN) {
    this->pos = this->pos - (this->zAxis * stepsize);
  }

  else if (key == GLFW_KEY_UP) {
    this->pos = this->pos + (this->zAxis * stepsize);
    // pos.print();
    // zAxis.print();
  } else if (key == GLFW_KEY_1) {
    setCamera(-0.0695547f, 4.16257f, 6.31657f, 2.5f, 1.0f, 2.0f, 0.0f, 1.0f,
              0.0f);
  } else if (key == GLFW_KEY_2) {
    setCamera(-1.5f, 5.60813f, 10.79671f, 6.7719f, 0.0813f, 2.59671f, 0.0f,
              1.0f, 0.0f);

  } else if (key == GLFW_KEY_3) {
    setCamera(2.2f, 4.95986f, 8.69347f, 2.2f, 1.0f, 2.0f, 0.0f, 1.0f, 0.0f);
  } else if (key == GLFW_KEY_4) {
    setCamera(-0.0695547f, 2.16257f, 4.31657f, 1.5f, 1.0f, 1.0f, 0.0f, 1.0f,
              0.0f);
  } else {
    std::cerr << key << " was not recognised and thrown away." << std::endl;
  }
}
void Camera::update(const double xpos, const double ypos) {
  setCamera(this->pos.x, this->pos.y, this->pos.z,
            this->pos.x + zAxis.x + xAxis.x * xpos,
            this->pos.y + zAxis.y + xAxis.y * xpos + ypos,
            this->pos.z + zAxis.z + xAxis.z * xpos, 0.0, 1.0, 0.0);
}

void Camera::setCamera(const float pos_x, const float pos_y, const float pos_z,
                       const float lookAt_x, const float lookAt_y,
                       const float lookAt_z, const float up_x, const float up_y,
                       const float up_z) {
  this->pos = glm::vec3(pos_x, pos_y, pos_z);
  glm::vec3 lookAt = glm::vec3(lookAt_x, lookAt_y, lookAt_z);
  this->up = glm::normalize(glm::vec3(up_x, up_y, up_z));
  this->zAxis = glm::normalize(lookAt - this->pos);
  this->xAxis = glm::cross(zAxis, up);
}

