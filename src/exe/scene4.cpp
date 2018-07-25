#include "launch.hpp"
using namespace std;

void scene(shared_ptr<ParticleSystem> const pPs,
           shared_ptr<CollisionObjects> const pCO,
           shared_ptr<std::vector<shared_ptr<Mesh>>> const meshes) {
  int x = 0;
  float xpos = GRID_POS_X + GRID_COLLISION_PLANE_OFFSET * GRID_SPACING,
        ypos = GRID_POS_Y + GRID_COLLISION_PLANE_OFFSET * GRID_SPACING,
        zpos = GRID_POS_Z + GRID_COLLISION_PLANE_OFFSET * GRID_SPACING;

  while (x < NUMOFPARTICLES) {
    float height = 0.5;
    float width =
        ((GRID_DIM_X - 2.0f * GRID_COLLISION_PLANE_OFFSET) * GRID_SPACING);
    // float radius = width/2.0f;
    float rand1 = (float(rand()) / 32727.0f) * width;
    float rand2 = (float(rand()) / 32727.0f) * height;
    float rand3 = (float(rand()) / 32727.0f) * width;
    // if(((rand1 -radius)*(rand1 -radius)+(rand2 -radius)*(rand2
    // -radius)+(rand3 -radius)*(rand3 -radius)) < (radius*radius)){
    pPs->particles->push_back(
        Particle(Vector3f(xpos + rand1, ypos + rand2, zpos + rand3)));
    x += 1;
    // }
  }

  shared_ptr<Mesh> sphere2 = make_shared<Mesh>();
  sphere2->LoadMesh("model/sphere.obj");
  sphere2->setPosition(0.4125f, 1.0125f, 2.8f);
  sphere2->setScale(0.2f, 0.2f, 0.2f);
  sphere2->setRotation(0, 0, 0);

  meshes->push_back(sphere2);
  pCO->colliders->push_back(Collider(sphere2, 0.2f, 1,
                                     Vector3f(10.0f, 0.0f, 0.0f),
                                     Vector3f(0.0f, 0.0f, 0.0f)));

  shared_ptr<Mesh> sphere = make_shared<Mesh>();
  sphere->LoadMesh("model/sphere.obj");
  sphere->setPosition(4.0125f, 1.0125f, 1.2f);
  sphere->setScale(0.2f, 0.2f, 0.2f);
  sphere->setRotation(0, 0, 0);

  meshes->push_back(sphere);
  pCO->colliders->push_back(Collider(sphere, 0.2f, 1,
                                     Vector3f(-10.0f, 0.0f, 0.0f),
                                     Vector3f(0.0f, 0.0f, 0.0f)));

  shared_ptr<Mesh> sphere3 = make_shared<Mesh>();
  sphere3->LoadMesh("model/sphere.obj");
  sphere3->setPosition(1.2f, 1.0125f, 0.4125f);
  sphere3->setScale(0.2f, 0.2f, 0.2f);
  sphere3->setRotation(0, 0, 0);

  meshes->push_back(sphere3);
  pCO->colliders->push_back(Collider(sphere3, 0.2f, 1,
                                     Vector3f(0.0f, 0.0f, 10.0f),
                                     Vector3f(0.0f, 0.0f, 0.0f)));
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

