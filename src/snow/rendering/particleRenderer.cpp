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
      PT(ParticleTechnique()),
      PTB(ParticleTechnique()) {}
void ParticleRenderer::fillBufferFromMeshes() {
  for (int i = 0; i < meshes->size(); i++) {
    (*meshes)[i]->initVBO();
  }
  // particlesystem->initVBO();
  // grid->initVBO();
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
  if (!PT.init("shader/particleShader.vert", "shader/particleShader.frag")) {
    printf("PT init failed");
  }

  if (!PTB.init("shader/borderShader.vert", "shader/borderShader.frag")) {
    printf("PTB init failed");
  }

  lighting.init("shader/shader.vert", "shader/shader.frag");
  /*
    const char* pVShadowMapFileName = "shader/m_shadow.vert";
    const char* pFShadowMapFileName = "shader/m_shadow.frag";

    vs.clear();
    fs.clear();

    if (!ReadFile(pVShadowMapFileName, vs)) {
      fprintf(stderr, "Error: vs\n");
      exit(1);
    };

    if (!ReadFile(pFShadowMapFileName, fs)) {
      fprintf(stderr, "Error: fs \n");
      exit(1);
    };
    SMFBO = ShadowMapBufferObject();
    SMFBO.Init(WINDOW_WIDTH, WINDOW_HEIGHT);
    SMT = ShadowMapTechnique();
    if (!SMT.init(vs, fs)) {
      printf("SMT init failed");
    }
    */
}
void ParticleRenderer::shadowMapPass() {
  /*
      SMFBO.BindForWriting();
      glClear(GL_DEPTH_BUFFER_BIT);


      SMT.plugTechnique();

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
  lightpos = Vector3f(4.0, 5.0f, 4.0f);

  // SMFBO.BindForReading(GL_TEXTURE1);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glClearColor(0.5, 0.5, 0.5, 0);

  //
  glm::mat4x4 matrix;
  glm::mat4x4 tmatrix;

  lighting.use();
  lighting.setSampler(0);
  lighting.setLight(lightpos, 0.1, Vector3f(1.0, 1.0, 1.0), 0.2);
  Vector3f specIntens(1.0, 1.0, 1.0);
  lighting.setSpecularIntensity(specIntens);
  lighting.setSpecularPower(10);
  lighting.setCameraPos(world.getCameraPos());
  for (int i = 0; i < meshes->size(); i++) {
    world.setPosition((*meshes)[i]->getPosition());
    world.setScale((*meshes)[i]->getScale());
    world.setRotation((*meshes)[i]->getRotation());
    matrix = glm::mat4x4(
        world.getModelMatrix()->m[0][0], world.getModelMatrix()->m[0][1],
        world.getModelMatrix()->m[0][2], world.getModelMatrix()->m[0][3],
        world.getModelMatrix()->m[1][0], world.getModelMatrix()->m[1][1],
        world.getModelMatrix()->m[1][2], world.getModelMatrix()->m[1][3],
        world.getModelMatrix()->m[2][0], world.getModelMatrix()->m[2][1],
        world.getModelMatrix()->m[2][2], world.getModelMatrix()->m[2][3],
        world.getModelMatrix()->m[3][0], world.getModelMatrix()->m[3][1],
        world.getModelMatrix()->m[3][2], world.getModelMatrix()->m[3][3]);
    tmatrix = glm::inverse(matrix);
    matrix = glm::transpose(tmatrix);

    // lighting.setShadowMapTexture(1);
    lighting.setWorldMatrix(world.getModelMatrix());
    lighting.setInverse(&matrix);
    lighting.setWVP(world.getMVP());
    lighting.setLightMVP(world.getMVP());  // TODO

    (*meshes)[i]->Render();
    // world.setCamera(lightpos.x,lightpos.y,lightpos.z,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);

    /*
    matrix= glm::mat4x4(    world.getModelMatrix()->m[0][0],
    world.getModelMatrix()->m[0][1], world.getModelMatrix()->m[0][2],
    world.getModelMatrix()->m[0][3], world.getModelMatrix()->m[1][0],
    world.getModelMatrix()->m[1][1], world.getModelMatrix()->m[1][2],
    world.getModelMatrix()->m[1][3], world.getModelMatrix()->m[2][0],
    world.getModelMatrix()->m[2][1], world.getModelMatrix()->m[2][2],
    world.getModelMatrix()->m[2][3], world.getModelMatrix()->m[3][0],
    world.getModelMatrix()->m[3][1], world.getModelMatrix()->m[3][2],
    world.getModelMatrix()->m[3][3]
        );
    tmatrix=glm::inverse(matrix);
    matrix=glm::transpose(tmatrix);

    lighting.setWorldMatrix(world.getModelMatrix());
    lighting.setInverse(&matrix);
    lighting.setWVP(world.getMVP());
    lighting.setLightMVP(world.getMVP());
    */
  }
  // world.setCamera(lightpos.x,lightpos.y,lightpos.z,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);

  // glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
  world.setPosition(Vector3f(0.0f, 0.0f, 0.0f));
  world.setScale(Vector3f(1.0f, 1.0f, 1.0f));
  world.setRotation(Vector3f(0.0f, 0.0f, 0.0f));

  PTB.use();
  PTB.setWVP(world.getMVP());
  grid->renderBorders();
  // particlesystem->updateVBOBuffer();
  PT.use();
  PT.setWVP(world.getMVP());

  particlesystem->render();

  glfwSwapBuffers(window);
  glfwPollEvents();
  // double timeS = glfwGetTime ();
  // grid->render();
  // double timeE = glfwGetTime();
  // std::cout << (timeE - timeS)*1000 << " ms for rendering the
  // grid."<<std::endl;
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

