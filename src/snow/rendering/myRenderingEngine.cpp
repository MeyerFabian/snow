#include "myRenderingEngine.hpp"

using namespace std;

GLFWwindow* window;

GLuint textureID;
shared_ptr<Texture> helitex;
Vector3f lightpos;

static float lighty = 0.0f;
void myRenderingEngine::fillBufferFromMeshes() {
  for (int i = 0; i < meshes->size(); i++) {
    (*meshes)[i]->initVBO();
  }
  // particlesystem->initVBO();
  // grid->initVBO();
}

void myRenderingEngine::initVBO() {
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

  GLFWWrapper::world.setPerspective(45, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f,
                                    50.0f);
  GLFWWrapper::world.setCamera(3.0, 3.5f, 14.0f, 2.5125f, 0.0f, 0.0f, 0.0f,
                               1.0f, 0.0f);
}

void myRenderingEngine::initShader() {
  particleImposter.init("shader/particleShader.vert",
                        "shader/particleShader.frag");
  gridBorderLines.init("shader/borderShader.vert", "shader/borderShader.frag");
  lighting.init("shader/shader.vert", "shader/shader.frag");
}
void myRenderingEngine::shadowMapPass() {}
void myRenderingEngine::renderPass() {
  lightpos = Vector3f(4.0, 5.0f, 4.0f);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glClearColor(0.5, 0.5, 0.5, 0);

  lighting.plugTechnique();

  /*
  lighting.setLight(lightpos, 0.1, Vector3f(1.0, 1.0, 1.0), 0.2);
  Vector3f specIntens(1.0, 1.0, 1.0);
  lighting.setSpecularIntensity(specIntens);
  lighting.setSpecularPower(10);
  lighting.setCameraPos(world.getCameraPos());
  /*/
  // lighting.uniform_update("gSampler", 0);
  lighting.uniform_update("gLightPosition", lightpos.x, lightpos.y, lightpos.z);
  lighting.uniform_update("gAmbient", 0.1f);
  lighting.uniform_update("gColor", 1.0f, 1.0f, 1.0f);
  lighting.uniform_update("gDiffuse", 0.2f);
  lighting.uniform_update("gSpecInt", 1.0f, 1.0f, 1.0f);
  lighting.uniform_update("gSpecPower", 10);
  lighting.uniform_update("gCameraPos", GLFWWrapper::world.getCameraPos().x,
                          GLFWWrapper::world.getCameraPos().y,
                          GLFWWrapper::world.getCameraPos().z);
  //*/
  for (int i = 0; i < meshes->size(); i++) {
    GLFWWrapper::world.setPosition((*meshes)[i]->getPosition());
    GLFWWrapper::world.setScale((*meshes)[i]->getScale());
    GLFWWrapper::world.setRotation((*meshes)[i]->getRotation());

    // lighting.setShadowMapTexture(1);

    lighting.uniform_update("gMVP", GLFWWrapper::world.getMVP());
    lighting.uniform_update("gModel", GLFWWrapper::world.getModelMatrix());
    (*meshes)[i]->Render();
  }
  GLFWWrapper::world.setPosition(Vector3f(0.0f, 0.0f, 0.0f));
  GLFWWrapper::world.setScale(Vector3f(1.0f, 1.0f, 1.0f));
  GLFWWrapper::world.setRotation(Vector3f(0.0f, 0.0f, 0.0f));

  gridBorderLines.plugTechnique();
  gridBorderLines.uniform_update("gMVP", GLFWWrapper::world.getMVP());
  grid->renderBorders();
  particleImposter.plugTechnique();
  particleImposter.uniform_update("gMVP", GLFWWrapper::world.getMVP());

  particlesystem->render();

  glfwSwapBuffers(GLFWWrapper::window);
  glfwPollEvents();
}

void myRenderingEngine::renderQueue() {
  // shadowMapPass();

  renderPass();
}

bool myRenderingEngine::init() {
  initVBO();

  initShader();
  // Textur anlegen

  return 0;
}

void myRenderingEngine::render() { renderQueue(); }
bool myRenderingEngine::shouldClose() {
  return !glfwWindowShouldClose(GLFWWrapper::window);
}

void myRenderingEngine::stop() {
  glfwDestroyWindow(GLFWWrapper::window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}

