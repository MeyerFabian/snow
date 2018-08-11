#include "particleRenderer.hpp"

using namespace std;
GLuint textureID;
shared_ptr<Texture> helitex;
Vector3f lightpos;

static float lighty = 0.0f;

ParticleRenderer::ParticleRenderer(
    std::shared_ptr<std::vector<shared_ptr<Mesh> > > const meshes,
    std::shared_ptr<ParticleSystem> const particles,
    std::shared_ptr<Grid> const grid)
    : Window_Context(),
      Renderer(meshes, particles, grid),
      lighting(LightingTechnique()),
      particleImposter(ParticleTechnique()),
      gridBorderLines(ParticleTechnique()) {}
void ParticleRenderer::fillBufferFromMeshes() {
  for (int i = 0; i < meshes->size(); i++) {
    (*meshes)[i]->initVBO();
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
      (*meshes)[0].Render();

  */
}
void ParticleRenderer::renderPass() {
  if (debug == true) {
    particlesystem->debug();
    grid->debug();
  }
  // pipeline light;
  lightpos = Vector3f(4.0f, 5.0f, 4.0f);

  // SMFBO.BindForReading(GL_TEXTURE1);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glClearColor(0.5, 0.5, 0.5, 0);

  lighting.use();
  lighting.updateUniform("gLightPosition", lightpos.x, lightpos.y, lightpos.z);
  lighting.updateUniform("gAmbient", 0.1f);
  lighting.updateUniform("gColor", 1.0f, 1.0f, 1.0f);
  lighting.updateUniform("gDiffuse", 0.2f);
  lighting.updateUniform("gSpecInt", 1.0f, 1.0f, 1.0f);
  lighting.updateUniform("gSpecPower", 10);
  lighting.updateUniform("gCameraPos", world.getCameraPos().x,
                         world.getCameraPos().y, world.getCameraPos().z);
  for (int i = 0; i < meshes->size(); i++) {
    world.setPosition((*meshes)[i]->getPosition());
    world.setScale((*meshes)[i]->getScale());
    world.setRotation((*meshes)[i]->getRotation());

    lighting.updateUniform("gMVP", world.getMVP());
    lighting.updateUniform("gModel", world.getModelMatrix());

    (*meshes)[i]->Render();
  }

  world.setPosition(Vector3f(0.0f, 0.0f, 0.0f));
  world.setScale(Vector3f(1.0f, 1.0f, 1.0f));
  world.setRotation(Vector3f(0.0f, 0.0f, 0.0f));

  gridBorderLines.use();
  gridBorderLines.updateUniform("gMVP", world.getMVP());
  grid->renderBorders();
  particleImposter.use();
  particleImposter.updateUniform("gMVP", world.getMVP());
  particlesystem->render();

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

