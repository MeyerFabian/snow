#include "../snow/utils/launchHelper/explicit.hpp"

int main() {
  Scene scene;

  /*
   * #define YOUNG_MODULUS 1.4e5
   * #define POISSON 0.41
   * #define HARDENING 10.0
   * #define CRIT_COMPRESSION 2.5e-2
   * #define CRIT_STRETCH 7.5e-3
   */

  unsigned int x = 0;
  float xpos = 5.0f, ypos = 1.7125f, zpos = 5.0f;
  while (x < 32 * 32 * 32) {
    float width = 1.0;
    float radius = width / 2.0f;
    float rand1 = (float(rand()) / 32727.0f) * width;
    float rand2 = (float(rand()) / 32727.0f) * width;
    float rand3 = (float(rand()) / 32727.0f) * width;
    float rand4 = (float(rand()) / 32727.0f) * width;
    if (((rand1 - radius) * (rand1 - radius) +
         (rand2 - radius) * (rand2 - radius) +
         (rand3 - radius) * (rand3 - radius)) < (radius * radius)) {
      float mass =
          (rand4 > 0.5) ? (((rand4 - 0.5) * 10.0f) + 1.0f) * 1.0e-4f : 5e-3f;

      scene.particleSys->particles.push_back(
          Particle(Vector3f(xpos + rand1, ypos + rand2, zpos + rand3),
                   Vector3f(0.0f, 0.0f, 0.0f), mass));

      x += 1;
    }
  }

  if (!launchSnow(scene)) {
    return 1;
  }

  return 0;
}

