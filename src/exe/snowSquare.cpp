#include "../snow/launchHelper/explicit.hpp"
using namespace std;
void scene(shared_ptr<ParticleSystem> const pPs,
           shared_ptr<CollisionObjects> const pCO,
           shared_ptr<std::vector<shared_ptr<Mesh>>> const meshes) {
  int x = 0;
  int y = 0;
  int z = 0;
  float xpos = 0.9625f, ypos = 0.72f, zpos = 0.9625;
  while (y < 32) {
    while (z < 32) {
      while (x < 32) {
        pPs->particles->push_back(Particle(
            Vector3f(xpos + ((float)x) * 0.025f, ypos + ((float)y) * 0.025f,
                     zpos + ((float)z) * 0.025f),
            Vector3i(0, 0, 0)));

        x += 1;
      }
      x = 0;
      z += 1;
    }
    z = 0;

    y += 1;
  }
}

class SnowSquareScene : public Scene {
 public:
  SnowSquareScene() = default;
  virtual void init(shared_ptr<ParticleSystem> const pPs,
                    shared_ptr<CollisionObjects> const pCO,
                    shared_ptr<std::vector<shared_ptr<Mesh>>> const meshes) {
    scene(pPs, pCO, meshes);
  }
};

int main() {
  SnowSquareScene scene;
  if (!launchSnow(scene)) {
    return 1;
  }

  return 0;
}

