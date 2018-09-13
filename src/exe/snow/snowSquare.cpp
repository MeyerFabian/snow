#include "../../snow/utils/launchHelper/explicit.hpp"

int main() {
  Scene scene;
  unsigned int x = 0;
  unsigned int y = 0;
  unsigned int z = 0;
  float xpos = 0.9625f, ypos = 0.72f, zpos = 0.9625;
  while (y < 32) {
    while (z < 32) {
      while (x < 32) {
        scene.particleSys->particles.push_back(Particle(
            glm::vec3(xpos + ((float)x) * 0.025f, ypos + ((float)y) * 0.025f,
                      zpos + ((float)z) * 0.025f),
            glm::vec3(0.0, 0.0, 0.0)));

        x += 1;
      }
      x = 0;
      z += 1;
    }
    z = 0;

    y += 1;
  }
  if (!launchSnow(scene)) {
    return 1;
  }

  return 0;
}

