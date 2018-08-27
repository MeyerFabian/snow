#ifndef PARTICLE_H
#define PARTICLE_H
#include <glm/glm.hpp>
struct Particle {
  Particle(
      glm::vec3 r = glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3 v = glm::vec3(0.0, 0.0, 0.0), float m = 1.25e-3f, float V = 0.0,
      glm::mat4 Fe = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                               0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),
      glm::mat4 Fp = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                               0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f))
      : position(r),
        velocity(v),
        mass(m),
        volume(V),
        forceElastic(Fe),
        forcePlastic(Fp) {}
  Particle(float x = .0f, float y = .0f, float z = .0f,
           glm::vec3 v = glm::vec3(0.0, 0.0, 0.0), float m = 1.25e-3f,
           float V = 0.0f, glm::mat4 Fe = glm::mat4(1.0f),
           glm::mat4 Fp = glm::mat4(1.0f))
      : position(x, y, z),
        velocity(v),
        mass(m),
        volume(V),
        forceElastic(Fe),
        forcePlastic(Fp) {}
  glm::vec3 position;
  glm::vec3 velocity;

  float mass;
  float volume;

  glm::mat4 forceElastic;
  glm::mat4 forcePlastic;
};

#endif  // PARTICLE_H

