#include "explicit.hpp"
#include "../benchmarker.hpp"
int launchSnow(Scene& scene) {
  std::cerr << "Custom Scene created!" << std::endl;
  srand(time(NULL));

  scene.grid =
      make_shared<Grid>(GRID_DIM_X, GRID_DIM_Y, GRID_DIM_Z, GRID_SPACING,
                        GRID_POS_X, GRID_POS_Y, GRID_POS_Z);
  shared_ptr<Mesh> halfplane = make_shared<Mesh>();
  halfplane->setPosition(
      GRID_POS_X + GRID_COLLISION_PLANE_OFFSET * GRID_SPACING,
      GRID_POS_Y + GRID_COLLISION_PLANE_OFFSET * GRID_SPACING,
      GRID_POS_Z + GRID_COLLISION_PLANE_OFFSET * GRID_SPACING);

  scene.colliderSys->colliders.push_back(Collider(halfplane, 0.0f, 0,
                                                  Vector3f(0.0f, 0.0f, 0.0f),
                                                  Vector3f(1.0f, 0.0f, 0.0f)));

  shared_ptr<Mesh> halfplane2 = make_shared<Mesh>();
  halfplane2->setPosition(
      GRID_POS_X + GRID_COLLISION_PLANE_OFFSET * GRID_SPACING,
      GRID_POS_Y + GRID_COLLISION_PLANE_OFFSET * GRID_SPACING,
      GRID_POS_Z + GRID_COLLISION_PLANE_OFFSET * GRID_SPACING);

  scene.colliderSys->colliders.push_back(Collider(halfplane2, 0.0f, 0,
                                                  Vector3f(0.0f, 0.0f, 0.0f),
                                                  Vector3f(0.0f, 1.0f, 0.0f)));

  shared_ptr<Mesh> halfplane3 = make_shared<Mesh>();
  halfplane3->setPosition(
      GRID_POS_X + GRID_COLLISION_PLANE_OFFSET * GRID_SPACING,
      GRID_POS_Y + GRID_COLLISION_PLANE_OFFSET * GRID_SPACING,
      GRID_POS_Z + GRID_COLLISION_PLANE_OFFSET * GRID_SPACING);

  scene.colliderSys->colliders.push_back(Collider(halfplane3, 0.0f, 0,
                                                  Vector3f(0.0f, 0.0f, 0.0f),
                                                  Vector3f(0.0f, 0.0f, 1.0f)));

  shared_ptr<Mesh> halfplane4 = make_shared<Mesh>();
  halfplane4->setPosition(
      GRID_POS_X + (GRID_DIM_X - GRID_COLLISION_PLANE_OFFSET) * GRID_SPACING,
      GRID_POS_Y + (GRID_DIM_Y - GRID_COLLISION_PLANE_OFFSET) * GRID_SPACING,
      GRID_POS_Z + (GRID_DIM_Z - GRID_COLLISION_PLANE_OFFSET) * GRID_SPACING);

  scene.colliderSys->colliders.push_back(Collider(halfplane4, 0.0f, 0,
                                                  Vector3f(0.0f, 0.0f, 0.0f),
                                                  Vector3f(-1.0f, 0.0f, 0.0f)));

  shared_ptr<Mesh> halfplane5 = make_shared<Mesh>();
  halfplane5->setPosition(
      GRID_POS_X + (GRID_DIM_X - GRID_COLLISION_PLANE_OFFSET) * GRID_SPACING,
      GRID_POS_Y + (GRID_DIM_Y - GRID_COLLISION_PLANE_OFFSET) * GRID_SPACING,
      GRID_POS_Z + (GRID_DIM_Z - GRID_COLLISION_PLANE_OFFSET) * GRID_SPACING);

  scene.colliderSys->colliders.push_back(Collider(halfplane5, 0.0f, 0,
                                                  Vector3f(0.0f, 0.0f, 0.0f),
                                                  Vector3f(0.0f, -1.0f, 0.0f)));

  shared_ptr<Mesh> halfplane6 = make_shared<Mesh>();
  halfplane6->setPosition(
      GRID_POS_X + (GRID_DIM_X - GRID_COLLISION_PLANE_OFFSET) * GRID_SPACING,
      GRID_POS_Y + (GRID_DIM_Y - GRID_COLLISION_PLANE_OFFSET) * GRID_SPACING,
      GRID_POS_Z + (GRID_DIM_Z - GRID_COLLISION_PLANE_OFFSET) * GRID_SPACING);

  scene.colliderSys->colliders.push_back(Collider(halfplane6, 0.0f, 0,
                                                  Vector3f(0.0f, 0.0f, 0.0f),
                                                  Vector3f(0.0, 0.0f, -1.0f)));

  /*
        Mesh halfplane;
halfplane.setPosition(0.7125f,0.7125f,0.7125f);
pCo->colliders->push_back(Collider(halfplane));
*/

  /*
Mesh quad;
quad.LoadMesh("model/box.obj");
quad.setPosition(5.0f,-3.0f,5.0f);
quad.setScale(10.0f,1.0f,10.0f);
quad.setRotation(0,0,0);
meshes->push_back(std::move(quad));

*/
  Benchmarker bench;
  shared_ptr<Renderer> const rE =
      make_shared<ParticleRenderer>(scene.get_renderable_scene());
  shared_ptr<TimeUpdate> const update =
      make_shared<ExplicitTimeUpdate>(scene.get_physical_scene());

  shared_ptr<physicEngine> const pE = make_shared<myPhysicEngine>(update);
  bool re_err =
      bench.benchmark_CPU("Renderderer init()", [&rE]() { return rE->init(); });

  bench.benchmark_CPU("Simulation init()", [&pE]() { return pE->init(); });

  if (re_err) {
    return 1;
  }

  double currentTime = glfwGetTime();  // gafferongames.com
  double accumulator = 0.0;
  std::cout << std::flush;
  while (rE->shouldClose()) {
    double newTime = glfwGetTime();
    double frameTime = newTime - currentTime;
    currentTime = newTime;
    accumulator += frameTime;
    while (accumulator >= STEP_DT) {
      bench.benchmark_CPU("Simulation update",
                          [&pE]() { return pE->update(PHYSIC_DT); });
      // pE->update(PHYSIC_DT);
      accumulator -= STEP_DT;
    }

    bench.benchmark_CPU("Renderer update", [&rE] { return rE->render(); });
  }
  bench.printStats();
  rE->stop();
  return 0;
}

