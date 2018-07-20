#ifndef MYRENDERINGENGINE_H
#define MYRENDERINGENGINE_H
#include "../rendering/renderingEngine.hpp"
class myRenderingEngine : public renderingEngine {
 public:
  myRenderingEngine(
      std::shared_ptr<std::vector<shared_ptr<Mesh> > > const meshes,
      std::shared_ptr<ParticleSystem> const particles,
      std::shared_ptr<Grid> const grid)
      : renderingEngine(meshes, particles, grid) {}
  virtual bool init();
  virtual void render();
  virtual bool shouldClose();
  virtual void stop();

  void fillBufferFromMeshes();
  void initVBO();
  void shadowMapPass();
  void renderPass();
  void renderQueue();
};
#endif  // MYRENDERINGENGINE_H
