#include "launch.hpp"
using namespace std;

void scene(shared_ptr<ParticleSystem> const pPs,
           shared_ptr<CollisionObjects> const pCO,
           shared_ptr<std::vector<shared_ptr<Mesh>>> const meshes) {
  Vector3f pos(5.0f, 0.0, 5.0f);
  Vector3f scale(12.5f * 1.259f, 12.5f * 1.259f, 12.5f * 1.259f);

  std::string filename = "model/bunny.voxel";
  pPs->initParticlesFromFile(filename, pos, scale);

  // 1. Test 91897 Particles 400kg/m Density 6.25Mass
  // 2. Test 190000 Particles 900kg/m Density 6.25 Mass SAME RESULT
  // 3. Test 91897 Particles 600kg/m Density 9.5Mass SAME RESULT
}
class SnowBunnyScene : public Scene {
 public:
  SnowBunnyScene() = default;
  virtual void init(shared_ptr<ParticleSystem> const pPs,
                    shared_ptr<CollisionObjects> const pCO,
                    shared_ptr<std::vector<shared_ptr<Mesh>>> const meshes) {
    scene(pPs, pCO, meshes);
  }
};

int main() {
  SnowBunnyScene scene;
  if (!launchSnow(scene)) {
    return 1;
  }

  return 0;
}

