#include "particleRenderer.hpp"

using namespace std;
GLuint textureID;

static float lighty = 0.0f;
ParticleRenderer::ParticleRenderer(RenderableScene&& scene)
    : Renderer(std::move(scene)) {}
pipeline ParticleRenderer::world;
void ParticleRenderer::fillBufferFromMeshes() {
  for (int i = 0; i < scene.meshSys->size(); i++) {
    (*scene.meshSys)[i]->initVBO();
  }
}

void ParticleRenderer::initVBO() {
  // VBO-Buffer Initialization
  glCullFace(GL_BACK);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_CULL_FACE);
  glEnable(GL_TEXTURE_2D);

  glEnable(GL_POINT_SPRITE);
  glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
  glfwSwapInterval(1);
  fillBufferFromMeshes();
  world.setPerspective(45, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 50.0f);
  world.setCamera(3.0, 3.5f, 14.0f, 2.5125f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
}

void ParticleRenderer::initShader() {
  particleImposter.init({});
  gridBorderLines.init({});

  Phong::UniformsStatic uniforms = {
      Vector3f(4.0f, 5.0f, 4.0f), 0.1f, Vector3f(1.0f, 1.0f, 1.0f), 0.2f,
      Vector3f(1.0f, 1.0f, 1.0f), 10,   world.getCameraPos()};
  basicLighting.init(std::move(uniforms));
}

void ParticleRenderer::renderPass() {
  BenchmarkerGPU::getInstance().time("#Ren:Meshes", [this]() {
    for (const auto& mesh : (*scene.meshSys)) {
      world.setPosition(mesh->getPosition());
      world.setScale(mesh->getScale());
      world.setRotation(mesh->getRotation());

      basicLighting.uniforms_update({world.getMVP(), world.getModelMatrix()});
      mesh->Render();
    }
  });

  world.setPosition(Vector3f(0.0f, 0.0f, 0.0f));
  world.setScale(Vector3f(1.0f, 1.0f, 1.0f));
  world.setRotation(Vector3f(0.0f, 0.0f, 0.0f));

  gridBorderLines.uniforms_update({world.getMVP()});
  BenchmarkerGPU::getInstance().time(
      "#Ren:GridLines", [this]() { return scene.grid->renderBorders(); });
  particleImposter.uniforms_update({world.getMVP()});

  BenchmarkerGPU::getInstance().time("#Ren:ParticleImposter", [this]() {
    return scene.particleSys->render();
  });
}

void ParticleRenderer::render() { renderPass(); }
void ParticleRenderer::init() {
  initVBO();

  initShader();
}

