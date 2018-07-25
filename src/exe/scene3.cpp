#include "launch.hpp"
using namespace std;
void scene(shared_ptr<ParticleSystem> const pPs,
           shared_ptr<CollisionObjects> const pCO,
           shared_ptr<std::vector<shared_ptr<Mesh>>> const meshes) {
  int x = 0;
  float xpos = 3.5f, ypos = 3.025f, zpos = 5.0f;
  while (x < 32 * 32 * 32) {
    float width = 1.1;
    float radius = width / 2.0f;
    float rand1 = (float(rand()) / 32727.0f) * width;
    float rand2 = (float(rand()) / 32727.0f) * width;
    float rand3 = (float(rand()) / 32727.0f) * width;
    if (((rand1 - radius) * (rand1 - radius) +
         (rand2 - radius) * (rand2 - radius) +
         (rand3 - radius) * (rand3 - radius)) < (radius * radius)) {
      pPs->particles->push_back(
          Particle(Vector3f(xpos + rand1, ypos + rand2, zpos + rand3),
                   Vector3i(0 * 1e6, 0 * 1e6, 0 * 1e6), 2.25e-3f));
      x += 1;
    }
  }
}
class Scene : public IScene {
 public:
  Scene() = default;
  ~Scene() = default;
  virtual void init(shared_ptr<ParticleSystem> const pPs,
                    shared_ptr<CollisionObjects> const pCO,
                    shared_ptr<std::vector<shared_ptr<Mesh>>> const meshes) {
    scene(pPs, pCO, meshes);
  }
};

int main() {
  Scene scene = Scene();
  if (!launchSnow(scene)) {
    return 1;
  }

  return 0;
}
