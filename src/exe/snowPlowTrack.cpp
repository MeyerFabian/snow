#include "../snow/utils/launchHelper/explicit.hpp"

int main() {
  Scene scene;
  int x = 0;
  float xpos = 0.8125,
        ypos = GRID_POS_Y + GRID_COLLISION_PLANE_OFFSET * GRID_SPACING,
        zpos = 2.4;
  while (x < 32 * 32 * 64) {
    float height = 0.5;
    float width = 2.0;
    float length = 0.8;
    // float radius = width/2.0f;
    float rand1 = (float(rand()) / 32727.0f) * width;
    float rand2 = (float(rand()) / 32727.0f) * height;
    float rand3 = (float(rand()) / 32727.0f) * length;
    // if(((rand1 -radius)*(rand1 -radius)+(rand2 -radius)*(rand2
    // -radius)+(rand3 -radius)*(rand3 -radius)) < (radius*radius)){
    scene.particleSys->particles.push_back(
        Particle(glm::vec3(xpos + rand1, ypos + rand2, zpos + rand3)));
    x += 1;
    // }
  }

  shared_ptr<Mesh> sphere = make_shared<Mesh>();
  sphere->LoadMesh("model/sphere.obj");
  sphere->setPosition(0.2125f, 1.1125f, 2.8f);
  sphere->setScale(0.3f, 0.3f, 0.3f);
  sphere->setRotation(0, 0, 0);

  scene.colliderSys->colliders.push_back(Collider(sphere, 0.3f, 1,
                                                  glm::vec3(5.0f, 0.0f, 0.0f),
                                                  glm::vec3(0.0f, 0.0f, 0.0f)));

  scene.meshSys->push_back(std::move(sphere));
  if (!launchSnow(scene)) {
    return 1;
  }

  return 0;
}

