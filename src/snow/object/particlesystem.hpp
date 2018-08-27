#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H
#define GLEW_STATIC
#include <GL/glew.h>

#include <fstream>
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>
#include "../utils/defines.hpp"
#include "particle.hpp"
class ParticleSystem {
 public:
  ~ParticleSystem() {
    glDeleteBuffers(1, &posB);
    glDeleteBuffers(1, &velB);
    glDeleteBuffers(1, &velBn);
    glDeleteBuffers(1, &deltaV0);
    glDeleteBuffers(1, &deltaV1);
    glDeleteBuffers(1, &deltaV2);
    glDeleteBuffers(1, &FEpB);
    glDeleteBuffers(1, &FPpB);
  }

  std::vector<Particle> particles;

  void render() const;
  void initParticlesFromFile(const std::string& filename, const glm::vec3& pos,
                             const glm::vec3& scale);
  void initSSBO();
  void updateSSBOBuffer();
  void debug() const;

 private:
  GLuint posB;
  GLuint velB;
  GLuint velBn;
  GLuint deltaV0;
  GLuint deltaV1;
  GLuint deltaV2;
  GLuint FEpB;
  GLuint FPpB;
};
#endif  // PARTICLESYSTEM_H

