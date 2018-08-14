#ifndef COLLISIONOBJECTS_H
#define COLLISIONOBJECTS_H
#define GLEW_STATIC
#include <GL/glew.h>

#include <memory>
#include <vector>
#include "collider.hpp"
#include "math3d.hpp"

#include "../utils/defines.hpp"
class CollisionObjects {
 public:
  ~CollisionObjects() {
    delete cPositions;
    delete cVelocities;
    delete cType;
    delete cFriction;
    delete cNormals;
  }

  std::vector<Collider> colliders;

  void initSSBO();
  void updateRenderBuffer(float dt);
  void debug();
  Vector4f* cPositions = nullptr;
  Vector4f* cVelocities = nullptr;
  GLint* cType = nullptr;
  float* cFriction = nullptr;
  Vector4f* cNormals = nullptr;

 private:
  GLuint posB;
  GLuint velB;
  GLuint norB;
  GLuint typeB;
  GLuint fricB;
};
#endif  // COLLISIONOBJECTS_H

