#include "myrenderingengine.h"
#define GLEW_STATIC
#include <glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../math3d.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "technique/lightingTechnique.h"
#include "pipeline/pipeline.h"
#include "../object/texture.h"
#include <glm.hpp>
#include "../object/mesh.h"
#include "technique/shadowMapTechnique.h"
#include "technique/shadowmapbufferobject.h"
#include "stb_image.h"
#include <memory>
#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 700
using namespace std;

GLFWwindow* window;

GLuint VBO;
GLuint IBO;

const char* pVSFileName = "shader/shader.vert";
const char* pFSFileName = "shader/shader.frag";
LightingTechnique lighting;


const char* pVShadowMapFileName = "shader/m_shadow.vert";
const char* pFShadowMapFileName = "shader/m_shadow.frag";
ShadowMapTechnique SMT;
ShadowMapBufferObject SMFBO;

GLuint textureID;
shared_ptr<Texture> helitex;


static float stepsize=0.2f;
static float rotation=0.0f;

Vector3f lightpos;
static float lighty=0.0f;


std::shared_ptr<std::vector< Mesh >> meshesToRender;

void fillBufferFromMeshes(){
    for(int i= 0;i< meshesToRender->size(); i++){
       (*meshesToRender)[i].initVBO();
    }
}

void initVBO(){

    //VBO-Buffer Initialization
    fillBufferFromMeshes();
    glCullFace(GL_BACK);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);

    helitex = make_shared<Texture>("textures/test.png");
    helitex->Load(GL_TEXTURE_2D);
}

void initShader(){



     string vs, fs;

       if(!ReadFile(pVSFileName,vs)){
         fprintf(stderr, "Error: vs\n");
          exit(1);
       };

       if(!ReadFile(pFSFileName,fs)){

        fprintf(stderr, "Error: fs \n");
        exit(1);
       };



        lighting= LightingTechnique();
        lighting.init(vs,fs);



       vs.clear();
       fs.clear();

       if(!ReadFile(pVShadowMapFileName,vs)){
         fprintf(stderr, "Error: vs\n");
          exit(1);
       };

       if(!ReadFile(pFShadowMapFileName,fs)){

        fprintf(stderr, "Error: fs \n");
        exit(1);
       };
       SMFBO = ShadowMapBufferObject();
       SMFBO.Init(WINDOW_WIDTH,WINDOW_HEIGHT);
       SMT=ShadowMapTechnique();
       if(!SMT.init(vs,fs)){
           printf("SMT init failed");
       }






    }
 void shadowMapPass(){

    SMFBO.BindForWriting();
    glClear(GL_DEPTH_BUFFER_BIT);


    SMT.plugTechnique();

    pipeline light;
    lightpos = Vector3f(0.0,lighty+ 3.0f,1.0f);

    light.setPosition(0.0f,0.0f,0.0f);
    light.setScale(0.003f,0.003f,0.003f);
    light.setRotation(0,rotation,0);
    light.setPerspective(45,WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 30.0f);
    light.setCamera(lightpos.x,lightpos.y,lightpos.z,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);

    SMT.setMVP(light.getMVP());
    (*meshesToRender)[0].Render();
    glBindFramebuffer(GL_FRAMEBUFFER,0);



}
 void renderPass(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glClearColor(1,1,1,0);

    SMFBO.BindForReading(GL_TEXTURE1);

    lighting.plugTechnique();
    pipeline world;

    //

    world.setPerspective(45,WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 30.0f);
    world.setCamera(0.0,3.0f,5.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);

    world.setPosition(0.0f,0.0f,0.0f);
    world.setScale(0.003f,0.003f,0.003f);
    world.setRotation(0,rotation,0);


    glm::mat4x4 matrix= glm::mat4x4(    world.getModelMatrix()->m[0][0], world.getModelMatrix()->m[0][1], world.getModelMatrix()->m[0][2], world.getModelMatrix()->m[0][3],
                                        world.getModelMatrix()->m[1][0], world.getModelMatrix()->m[1][1], world.getModelMatrix()->m[1][2], world.getModelMatrix()->m[1][3],
                                        world.getModelMatrix()->m[2][0], world.getModelMatrix()->m[2][1], world.getModelMatrix()->m[2][2], world.getModelMatrix()->m[2][3],
                                        world.getModelMatrix()->m[3][0], world.getModelMatrix()->m[3][1], world.getModelMatrix()->m[3][2], world.getModelMatrix()->m[3][3]
            );
    glm::mat4x4 tmatrix=glm::inverse(matrix);
    matrix=glm::transpose(tmatrix);

    lighting.setSampler(0);
    lighting.setShadowMapTexture(1);
    lighting.setLight(lightpos,0.1,Vector3f(1.0,1.0,1.0) ,0.90);
    lighting.setWorldMatrix(world.getModelMatrix());
    lighting.setInverse(&matrix);
    lighting.setWVP(world.getMVP());
    lighting.setCameraPos(world.getCameraPos());
    Vector3f specIntens(1.0,1.0,1.0);
    lighting.setSpecularIntensity(specIntens);
    lighting.setSpecularPower(10);

    world.setCamera(lightpos.x,lightpos.y,lightpos.z,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);
    lighting.setLightMVP(world.getMVP());//TODO




    (*meshesToRender)[0].Render();
    world.setPosition(0,-3.0f,0.0f);
    world.setScale(10.0f,1.0f,10.0f);
    world.setRotation(0,rotation,0);

    world.setCamera(0.0,3.0f,5.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);
    //world.setPerspective(45,WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 30.0f);
    //world.setCamera(0.0,3.0,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);

    matrix= glm::mat4x4(    world.getModelMatrix()->m[0][0], world.getModelMatrix()->m[0][1], world.getModelMatrix()->m[0][2], world.getModelMatrix()->m[0][3],
                                    world.getModelMatrix()->m[1][0], world.getModelMatrix()->m[1][1], world.getModelMatrix()->m[1][2], world.getModelMatrix()->m[1][3],
                                    world.getModelMatrix()->m[2][0], world.getModelMatrix()->m[2][1], world.getModelMatrix()->m[2][2], world.getModelMatrix()->m[2][3],
                                    world.getModelMatrix()->m[3][0], world.getModelMatrix()->m[3][1], world.getModelMatrix()->m[3][2], world.getModelMatrix()->m[3][3]
        );
    tmatrix=glm::inverse(matrix);
    matrix=glm::transpose(tmatrix);

    lighting.setWorldMatrix(world.getModelMatrix());
    lighting.setInverse(&matrix);
    lighting.setWVP(world.getMVP());
    world.setCamera(lightpos.x,lightpos.y,lightpos.z,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);
    lighting.setLightMVP(world.getMVP());
    helitex->bind(GL_TEXTURE0);
    (*meshesToRender)[1].Render();

}

 void renderQueue(){

     shadowMapPass();

     renderPass();

}

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
    else if(key == GLFW_KEY_Q ){
        if(action == GLFW_PRESS)
        rotation += 0.1f;
    }
    else if(key == GLFW_KEY_E ){
        if(action == GLFW_PRESS)
        rotation -= 0.1f;
    }
    else if(key == GLFW_KEY_W){
        if(action == GLFW_PRESS)
        lighty += 1.0f;
    }
    else if(key == GLFW_KEY_S){
        if(action == GLFW_PRESS)
        lighty -= 1.0f;
    }
    else{
    //world.update(key,stepsize);
    }
}

bool myRenderingEngine::init(std::shared_ptr<std::vector< Mesh >> meshes){

    meshesToRender  = meshes;

    //GLFW INIT: ORDER IS IMPORTANT
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
    exit(EXIT_FAILURE);
    window = glfwCreateWindow(WINDOW_WIDTH,WINDOW_HEIGHT, "OpenGL Project", NULL, NULL);
    if (!window){
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    //GLEW INIT
    GLenum err = glewInit();

    if (err != GLEW_OK)
    {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(err));

        return 1;
    }

    initVBO();

    initShader();
//Textur anlegen

    return 0;
}

void myRenderingEngine::render(){
    renderQueue();

    glfwSwapBuffers(window);
    glfwPollEvents();
}
bool myRenderingEngine::shouldClose(){

    return !glfwWindowShouldClose(window);
}

void myRenderingEngine::stop(){
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

