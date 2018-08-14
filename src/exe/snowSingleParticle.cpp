#include "../snow/utils/launchHelper/explicit.hpp"

int main() {
  Scene scene;
  unsigned int x = 0;
  float xpos = 5.0f, ypos = 1.7125f, zpos = 5.0f;
  while (x < 1) {
    float width = 1.0;
    float radius = width / 2.0f;
    float rand1 = (float(rand()) / 32727.0f) * width;
    float rand2 = (float(rand()) / 32727.0f) * width;
    float rand3 = (float(rand()) / 32727.0f) * width;
    if (((rand1 - radius) * (rand1 - radius) +
         (rand2 - radius) * (rand2 - radius) +
         (rand3 - radius) * (rand3 - radius)) < (radius * radius)) {
      float mass = 1.25e-3f;
      scene.particleSys->particles.push_back(
          Particle(Vector3f(xpos + rand1, ypos + rand2, zpos + rand3),
                   Vector3i(0, 0, 0), mass));
      x += 1;
    }
  }
  if (!launchSnow(scene)) {
    return 1;
  }

  return 0;
}

