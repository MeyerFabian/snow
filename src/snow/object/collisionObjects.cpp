#include "collisionObjects.hpp"
#include <iostream>

/**
 * STUB
 */

void CollisionObjects::debug() {
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, posB);
  std::cout << "pos" << std::endl;
  auto p = (glm::vec4*)(glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0,
                                         sizeof(glm::vec4) * colliders.size(),
                                         GL_MAP_READ_BIT));
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

void CollisionObjects::initSSBO() {
  glGenBuffers(1, &posB);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, posB);
  glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * colliders.size(),
               NULL, GL_STREAM_DRAW);
  auto cPositions = (glm::vec4*)(glMapBufferRange(
      GL_SHADER_STORAGE_BUFFER, 0, sizeof(glm::vec4) * colliders.size(),
      GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
  for (int i = 0; i < colliders.size(); i++) {
    glm::vec4 data(colliders.at(i).mesh->getPosition(),
                   (float)colliders.at(i).type);
    cPositions[i] = data;
  }
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, COLLIDER_POS_BUFFER, posB);

  glGenBuffers(1, &velB);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, velB);
  glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * colliders.size(),
               NULL, GL_STATIC_DRAW);
  auto cVelocities = (glm::vec4*)(glMapBufferRange(
      GL_SHADER_STORAGE_BUFFER, 0, sizeof(glm::vec4) * colliders.size(),
      GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
  for (int i = 0; i < colliders.size(); i++) {
    cVelocities[i] = colliders.at(i).velocity;
  }
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, COLLIDER_VEL_BUFFER, velB);

  glGenBuffers(1, &norB);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, norB);
  glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * colliders.size(),
               NULL, GL_STATIC_DRAW);
  auto cNormals = (glm::vec4*)(glMapBufferRange(
      GL_SHADER_STORAGE_BUFFER, 0, sizeof(glm::vec4) * colliders.size(),
      GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
  for (int i = 0; i < colliders.size(); i++) {
    glm::vec4 data(colliders.at(i).normal, colliders.at(i).friction);
    cNormals[i] = data;
  }
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, COLLIDER_NOR_BUFFER, norB);
}
/**
 * @brief CollisionObjects::updateRenderBuffer
 * @param dt
 * Updates the uniform variables for render calls, not an optimal way of doing
 * it. You could pass the ssbo containing positions from the compute shader to
 * the vertexshader and give every mesh a uniform which indicates a position and
 * work exactly on the same data, while this solution does the same thing on GPU
 * and CPU, which is not recommandable. But works for now.
 */
void CollisionObjects::updateRenderBuffer(float dt) {
  for (int i = 0; i < colliders.size(); i++) {
    glm::vec3 position = colliders.at(i).mesh->getPosition();
    glm::vec3 velocity = colliders.at(i).velocity;

    glm::vec3 uPos(position + velocity * dt);
    colliders.at(i).mesh->setPosition(uPos);
    if (colliders.at(i).type == 1) {
      colliders.at(i).velocity = glm::vec4(0.0f, dt * 0.0f, 0.0f, 0.0f);
    }
  }
}

