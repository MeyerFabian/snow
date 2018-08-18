#include "particleRenderer.hpp"

using namespace std;
GLuint textureID;
shared_ptr<Texture> helitex;
Vector3f lightpos;

static float lighty = 0.0f;

ParticleRenderer::ParticleRenderer(RenderableScene&& scene)
    : Window_Context(), Renderer(std::move(scene)) {}

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

  helitex = make_shared<Texture>("textures/test.png");
  helitex->Load(GL_TEXTURE_2D);

  world.setPerspective(45, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 50.0f);
  world.setCamera(3.0, 3.5f, 14.0f, 2.5125f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
}

void ParticleRenderer::initShader() {
  particleImposter.init("shader/particleShader.vert",
                        "shader/particleShader.frag");
  gridBorderLines.init("shader/borderShader.vert", "shader/borderShader.frag");
  lighting.init("shader/shader.vert", "shader/shader.frag");

  //  SMT.init("shader/m_shadow.vert", "shader/m_shadow.frag")
}
void ParticleRenderer::shadowMapPass() {
  /*
      SMFBO.BindForWriting();
      glClear(GL_DEPTH_BUFFER_BIT);


      SMT.use();

      pipeline light;
      lightpos = Vector3f(0.0,lighty+ 3.0f,1.0f);

      light.setPosition(0.0f,0.0f,0.0f);
      light.setScale(0.003f,0.003f,0.003f);
      light.setRotation(0,0,0);
      light.setPerspective(45,WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 30.0f);
      light.setCamera(lightpos.x,lightpos.y,lightpos.z,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);

      SMT.setMVP(light.getMVP());
      (*scene.meshSys)[0].Render();

  */
}
void ParticleRenderer::renderPass() {
  // pipeline light;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.5, 0.5, 0.5, 0);

  lightpos = Vector3f(4.0f, 5.0f, 4.0f);

  // SMFBO.BindForReading(GL_TEXTURE1);

  lighting.use();
  lighting.uniform_update("gLightPosition", lightpos.x, lightpos.y, lightpos.z);
  lighting.uniform_update("gAmbient", 0.1f);
  lighting.uniform_update("gColor", 1.0f, 1.0f, 1.0f);
  lighting.uniform_update("gDiffuse", 0.2f);
  lighting.uniform_update("gSpecInt", 1.0f, 1.0f, 1.0f);
  lighting.uniform_update("gSpecPower", 10);
  auto camera = world.getCameraPos();
  lighting.uniform_update("gCameraPos", camera.x, camera.y, camera.z);
  for (int i = 0; i < scene.meshSys->size(); i++) {
    world.setPosition((*scene.meshSys)[i]->getPosition());
    world.setScale((*scene.meshSys)[i]->getScale());
    world.setRotation((*scene.meshSys)[i]->getRotation());

    lighting.uniform_update("gMVP", world.getMVP());
    lighting.uniform_update("gModel", world.getModelMatrix());

    (*scene.meshSys)[i]->Render();
  }

  world.setPosition(Vector3f(0.0f, 0.0f, 0.0f));
  world.setScale(Vector3f(1.0f, 1.0f, 1.0f));
  world.setRotation(Vector3f(0.0f, 0.0f, 0.0f));

  gridBorderLines.use();
  gridBorderLines.uniform_update("gMVP", world.getMVP());
  scene.grid->renderBorders();
  particleImposter.use();
  particleImposter.uniform_update("gMVP", world.getMVP());
  scene.particleSys->render();

  glfwSwapBuffers(window);
  glfwPollEvents();
}

void ParticleRenderer::renderQueue() {
  // shadowMapPass();

  renderPass();
}
bool ParticleRenderer::init() {
  initVBO();

  initShader();
  // Textur anlegen

  return 0;
}

void ParticleRenderer::render() { renderQueue(); }
bool ParticleRenderer::shouldClose() { return !glfwWindowShouldClose(window); }

void ParticleRenderer::stop() {
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}

