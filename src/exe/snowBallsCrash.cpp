#include "../snow/launchHelper/explicit.hpp"
using namespace std;
void scene(shared_ptr<ParticleSystem> const pPs,
           shared_ptr<CollisionObjects> const pCO,
           shared_ptr<std::vector<shared_ptr<Mesh>>> const meshes) {
  int x = 0;
  float xpos = 1.0f, ypos = 1.7125f, zpos = 5.0f;
  while (x < 32 * 32 * 16) {
    float width = 0.8;
    float radius = width / 2.0f;
    float rand1 = (float(rand()) / 32727.0f) * width;
    float rand2 = (float(rand()) / 32727.0f) * width;
    float rand3 = (float(rand()) / 32727.0f) * width;

    float rand4 = (float(rand()) / 32727.0f) * width;
    if (((rand1 - radius) * (rand1 - radius) +
         (rand2 - radius) * (rand2 - radius) +
         (rand3 - radius) * (rand3 - radius)) < (radius * radius)) {
      // float mass = (rand4>0.5)? rand4*6.0f*5.0e-4f:10e-3f;
      float mass =
          (rand4 > 0.5) ? (((rand4 - 0.5) * 10.0f) + 1.0f) * 8.0e-4f : 10e-3f;
      pPs->particles->push_back(
          Particle(Vector3f(xpos + rand1, ypos + rand2, zpos + rand3),
                   Vector3i(10 * 1e8, 0 * 1e8, 1 * 1e8), mass));
      x += 1;
    }
  }

  x = 0;
  xpos = 4.5f, ypos = 1.7125f, zpos = 5.5f;
  while (x < 32 * 32 * 16) {
    float width = 0.8;
    float radius = width / 2.0f;
    float rand1 = (float(rand()) / 32727.0f) * width;
    float rand2 = (float(rand()) / 32727.0f) * width;
    float rand3 = (float(rand()) / 32727.0f) * width;

    float rand4 = (float(rand()) / 32727.0f) * width;
    if (((rand1 - radius) * (rand1 - radius) +
         (rand2 - radius) * (rand2 - radius) +
         (rand3 - radius) * (rand3 - radius)) < (radius * radius)) {
      float mass =
          (rand4 > 0.5) ? (((rand4 - 0.5) * 10.0f) + 1.0f) * 8.0e-4f : 10e-3f;
      pPs->particles->push_back(
          Particle(Vector3f(xpos + rand1, ypos + rand2, zpos + rand3),
                   Vector3i(-12 * 1e8, 1 * 1e8, -5 * 1e8), mass));
      x += 1;
    }
  }
}
class SnowBallsCrashScene : public Scene {
 public:
  virtual void init(shared_ptr<ParticleSystem> const pPs,
                    shared_ptr<CollisionObjects> const pCO,
                    shared_ptr<std::vector<shared_ptr<Mesh>>> const meshes) {
    scene(pPs, pCO, meshes);
  }
};

int main() {
  SnowBallsCrashScene scene;
  if (!launchSnow(scene)) {
    return 1;
  }

  return 0;
}

