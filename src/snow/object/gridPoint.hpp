#ifndef GRIDPOINT_H
#define GRIDPOINT_H
#include <glm/glm.hpp>
struct GridPoint {
  GridPoint(glm::vec3 v = glm::vec3(0.0f, 0.0f, 0.0f), float m = 0,
            glm::mat3 Fe = glm::mat3(1.0f), glm::mat3 Fp = glm::mat3(1.0f))
      : velocity(v), mass(m), forceElastic(Fe), forcePlastic(Fp) {}

  glm::vec3 velocity;

  float mass;

  glm::mat3 forceElastic;
  glm::mat3 forcePlastic;
};

#endif  // GRIDPOINT_H

