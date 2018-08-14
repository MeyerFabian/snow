#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H
#define GLEW_STATIC
#include <GL/glew.h>

#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include "../utils/defines.hpp"
#include "math3d.hpp"
#include "particle.hpp"
class ParticleSystem {
 public:
  ~ParticleSystem() {
    delete pPositions;
    delete pVelocities;
    delete pForcesE;
    delete pForcesP;
    delete pVelocitiesn;
    delete pDeltaV0;
    delete pDeltaV1;
    delete pDeltaV2;
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

  void render();
  void initParticlesFromFile(const std::string& filename, const Vector3f& pos,
                             const Vector3f& scale);
  void initSSBO();
  void updateSSBOBuffer();
  void debug();
  Vector4f* pPositions = nullptr;
  Vector4i* pVelocities = nullptr;
  Matrix4f* pForcesE = nullptr;
  Matrix4f* pForcesP = nullptr;
  Vector4i* pVelocitiesn = nullptr;
  Vector4i* pDeltaV0 = nullptr;
  Vector4i* pDeltaV1 = nullptr;
  Vector4i* pDeltaV2 = nullptr;

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

