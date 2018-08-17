#include "myRenderingEngine.hpp"

using namespace std;

GLFWwindow* window;
const char* pVSFileName = "shader/shader.vert";
const char* pFSFileName = "shader/shader.frag";

const char* pVSParticleFileName = "shader/particleShader.vert";
const char* pFSParticleFileName = "shader/particleShader.frag";

const char* pVSBorderFileName = "shader/borderShader.vert";
const char* pFSBorderFileName = "shader/borderShader.frag";

pipeline world;
GLuint textureID;
shared_ptr<Texture> helitex;

static float anglesize = 0.001f;
static float stepsize = 0.05f;
static float rotation = 0.0f;

Vector3f lightpos;
static float lighty = 0.0f;
bool debug;
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

  world.setPerspective(45, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 50.0f);
  world.setCamera(3.0, 3.5f, 14.0f, 2.5125f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
}

void myRenderingEngine::initShader() {
  string vs, fs, gs;
  vs.clear();
  fs.clear();
  gs.clear();
  if (!ReadFile(pVSParticleFileName, vs)) {
    fprintf(stderr, "Error: vs\n");
    exit(1);
  };

  if (!ReadFile(pFSParticleFileName, fs)) {
    fprintf(stderr, "Error: fs \n");
    exit(1);
  };

  if (!PT.init(vs, fs)) {
    printf("PT init failed");
  }

  vs.clear();
  fs.clear();
  if (!ReadFile(pVSBorderFileName, vs)) {
    fprintf(stderr, "Error: vs\n");
    exit(1);
  };

  if (!ReadFile(pFSBorderFileName, fs)) {
    fprintf(stderr, "Error: fs \n");
    exit(1);
  };

  if (!PTB.init(vs, fs)) {
    printf("PT init failed");
  }

  vs.clear();
  fs.clear();
  if (!ReadFile(pVSFileName, vs)) {
    fprintf(stderr, "Error: vs\n");
    exit(1);
  };

  if (!ReadFile(pFSFileName, fs)) {
    fprintf(stderr, "Error: fs \n");
    exit(1);
  };

  lighting.init(vs, fs);
}
void myRenderingEngine::shadowMapPass() {}
void myRenderingEngine::renderPass() {
  if (debug == true) {
    particlesystem->debug();
    grid->debug();
  }
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
  lighting.uniform_update("gCameraPos", world.getCameraPos().x,
                          world.getCameraPos().y, world.getCameraPos().z);
  //*/
  for (int i = 0; i < meshes->size(); i++) {
    world.setPosition((*meshes)[i]->getPosition());
    world.setScale((*meshes)[i]->getScale());
    world.setRotation((*meshes)[i]->getRotation());

    // lighting.setShadowMapTexture(1);

    lighting.uniform_update("gMVP", world.getMVP());
    lighting.uniform_update("gModel", world.getModelMatrix());
    (*meshes)[i]->Render();
  }
  world.setPosition(Vector3f(0.0f, 0.0f, 0.0f));
  world.setScale(Vector3f(1.0f, 1.0f, 1.0f));
  world.setRotation(Vector3f(0.0f, 0.0f, 0.0f));

  PTB.plugTechnique();
  PTB.uniform_update("gMVP", world.getMVP());
  grid->renderBorders();
  PT.plugTechnique();
  PT.uniform_update("gMVP", world.getMVP());

  particlesystem->render();

  glfwSwapBuffers(window);
  glfwPollEvents();
}

void myRenderingEngine::renderQueue() {
  // shadowMapPass();

  renderPass();
}

static void error_callback(int error, const char* description) {
  fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action,
                         int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
  else if (key == GLFW_KEY_Q) {
    if (action == GLFW_PRESS) {
    }
  } else if (key == GLFW_KEY_E) {
    if (action == GLFW_PRESS) {
    }
  } else if (key == GLFW_KEY_W) {
    if (action == GLFW_PRESS) {
    }

  } else if (key == GLFW_KEY_S) {
    if (action == GLFW_PRESS) {
    }
  } else if (key == GLFW_KEY_D) {
    if (action == GLFW_PRESS) {
      debug = true;
    } else if (action == GLFW_RELEASE) {
      debug = false;
    }
  } else {
    world.update(key, stepsize);
  }
}
static void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
  double mouse_x = (xpos - WINDOW_WIDTH / 2) * anglesize;
  double mouse_y = (-ypos + WINDOW_HEIGHT / 2) * anglesize;

  world.update(mouse_x, mouse_y);
  glfwSetCursorPos(window, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
}
bool myRenderingEngine::init() {
  // GLFW INIT: ORDER IS IMPORTANT
  glfwSetErrorCallback(error_callback);
  if (!glfwInit()) exit(EXIT_FAILURE);

  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Project", NULL,
                            NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPos(window, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
  glfwSetCursorPosCallback(window, mouse_callback);

  // GLEW INIT
  GLenum err = glewInit();

  if (err != GLEW_OK) {
    fprintf(stderr, "Error: '%s'\n", glewGetErrorString(err));

    return 1;
  }
  initVBO();

  initShader();
  // Textur anlegen

  return 0;
}

void myRenderingEngine::render() { renderQueue(); }
bool myRenderingEngine::shouldClose() { return !glfwWindowShouldClose(window); }

void myRenderingEngine::stop() {
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}

