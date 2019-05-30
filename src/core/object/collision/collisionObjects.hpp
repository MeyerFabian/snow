#ifndef COLLISIONOBJECTS_H
#define COLLISIONOBJECTS_H
#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <memory>
#include <vector>
#include "collider.hpp"

#include "../../utils/defines.hpp"

/* TODO: needs rework old buffers */
class CollisionObjects {
 public:
  std::vector<Collider> colliders;

  void initSSBO();
  void updateRenderBuffer(float dt);
  void debug();

 private:
  GLuint posB;
  GLuint velB;
  GLuint norB;
  GLuint typeB;
  GLuint fricB;
};
#endif  // COLLISIONOBJECTS_H

