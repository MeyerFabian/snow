
#include "rendering/myRenderingEngine.h"
#include "simulation/myPhysicEngine.h"
#include <iostream>
#include <memory>
#include "object/mesh.h"
#include <vector>
#include "object/grid.h"
#include "object/particlesystem.h"
#include "simulation/timeUpdate.h"
#include "defines.h"
#include "GLFW/glfw3.h"
#include <time.h>
#include "math.h"
using namespace  std;

double dt =  0.01666666666;

double t = 0.0;

void scene1(shared_ptr<ParticleSystem> const pPs){
    int x = 0;
    float xpos=0.8f,ypos=1.7125f,zpos=5.0f;
    while(x<32*32*32){
        float width = 0.8;
        float radius = width/2.0f;
        float rand1=(float(rand())/32727.0f)*width;
        float rand2=(float(rand())/32727.0f)*width;
        float rand3=(float(rand())/32727.0f)*width;
        if(((rand1 -radius)*(rand1 -radius)+(rand2 -radius)*(rand2 -radius)+(rand3 -radius)*(rand3 -radius)) < (radius*radius)){
        pPs->particles->push_back(Particle(Vector3f(
                                               xpos + rand1,
                                               ypos + rand2,
                                               zpos + rand3)));
        x+=1;
        }
    }
}
void scene2(shared_ptr<ParticleSystem> const pPs){
    int x = 0;
    float xpos=2.0f,ypos=1.7125f,zpos=5.0f;
    while(x<32*32*16){
        float width = 0.8;
        float radius = width/2.0f;
        float rand1=(float(rand())/32727.0f)*width;
        float rand2=(float(rand())/32727.0f)*width;
        float rand3=(float(rand())/32727.0f)*width;
        if(((rand1 -radius)*(rand1 -radius)+(rand2 -radius)*(rand2 -radius)+(rand3 -radius)*(rand3 -radius)) < (radius*radius)){
        pPs->particles->push_back(Particle(Vector3f(
                                               xpos + rand1,
                                               ypos + rand2,
                                               zpos + rand3),Vector3i(10*1000000,1*1000000,1*1000000)));
        x+=1;
        }
    }

    x = 0;
    xpos=3.5f,ypos=1.7125f,zpos=5.0f;
    while(x<32*32*16){
        float width = 0.8;
        float radius = width/2.0f;
        float rand1=(float(rand())/32727.0f)*width;
        float rand2=(float(rand())/32727.0f)*width;
        float rand3=(float(rand())/32727.0f)*width;
        if(((rand1 -radius)*(rand1 -radius)+(rand2 -radius)*(rand2 -radius)+(rand3 -radius)*(rand3 -radius)) < (radius*radius)){
        pPs->particles->push_back(Particle(Vector3f(
                                               xpos + rand1,
                                               ypos + rand2,
                                               zpos + rand3),Vector3i(-30*1000000,15*1000000,-1*1000000)));
        x+=1;
        }
    }
}
void scene3(shared_ptr<ParticleSystem> const pPs){
    int x = 0;
    float xpos=1.5f,ypos=2.125f,zpos=5.0f;
    while(x<32*32*32){
        float width = 0.8;
        float radius = width/2.0f;
        float rand1=(float(rand())/32727.0f)*width;
        float rand2=(float(rand())/32727.0f)*width;
        float rand3=(float(rand())/32727.0f)*width;
        if(((rand1 -radius)*(rand1 -radius)+(rand2 -radius)*(rand2 -radius)+(rand3 -radius)*(rand3 -radius)) < (radius*radius)){
        pPs->particles->push_back(Particle(Vector3f(
                                               xpos + rand1,
                                               ypos + rand2,
                                               zpos + rand3),Vector3i(-1*1000000,0,1*1000000)));
        x+=1;
        }
    }

}
int launchSnow(){
    shared_ptr<Grid>const  grid = make_shared<Grid >(GRID_DIM_X,GRID_DIM_Y,GRID_DIM_Z, GRID_SPACING, GRID_POS_X,GRID_POS_Y,GRID_POS_Z);


    shared_ptr<ParticleSystem> const pPs= make_shared<ParticleSystem > ();

     srand (time(NULL));
     //scene1(pPs);
     //scene2(pPs);
     scene2(pPs);
/*
    for(int x = 0; x < 32; x+=1){
        xpos += 0.025f;
        for(int y = 0; y < 32; y+=1){
            ypos += 0.025f;
            for(int z = 0; z < 32; z+=1){
                zpos += 0.025f;
                pPs->particles->push_back(Particle(Vector3f(xpos,ypos,zpos)));
            }
            zpos = 5.0f;
        }
        ypos = 2.5125f;
    }
*/

/*
    for(int x = 0; x<32*32*32; x+=1){
        float rand1=(float(rand())/32727.0f)*0.4;
        float rand2=(float(rand())/32727.0f)*0.4;
        float rand3=(float(rand())/32727.0f)*0.4;
        float randpi=(float(rand())/32727.0f)*M_PI;
        float rand2pi=(float(rand())/32727.0f)*2.0f*M_PI;
        pPs->particles->push_back(Particle(Vector3f(
                                               xpos + rand1*sin(randpi)*cos(rand2pi),
                                               ypos + rand2*sin(randpi)*sin(rand2pi),
                                               zpos + rand3*cos(randpi))));
    }
*/
/*

    std:: cout <<(float(rand())/32727.0f)*0.8<<std::endl;
    std:: cout <<(float(rand())/32727.0f)*0.8<<std::endl;
    std:: cout <<(float(rand())/32727.0f)*0.8<<std::endl;
    std:: cout <<(float(rand())/32727.0f)*0.8<<std::endl;
    std:: cout <<(float(rand())/32727.0f)*0.8<<std::endl;
    std:: cout <<(float(rand())/32727.0f)*0.8<<std::endl;
    std:: cout <<(float(rand())/32727.0f)*0.8<<std::endl;
    std:: cout <<(float(rand())/32727.0f)*0.8<<std::endl;
*/
    shared_ptr<std::vector<Mesh> > const  meshes = make_shared<std::vector<Mesh> >();

    Mesh jeep;
    jeep.LoadMesh("model/jeep.obj");
    jeep.setPosition(4.0f,-0.25f,4.0f);
    jeep.setScale(0.003f,0.003f,0.003f);
    jeep.setRotation(0,0,0);
    Mesh quad;
    quad.LoadMesh("model/box.obj");
    quad.setPosition(5.0f,-3.0f,5.0f);
    quad.setScale(10.0f,1.0f,10.0f);
    quad.setRotation(0,0,0);
    meshes->push_back(std::move(jeep));
    meshes->push_back(std::move(quad));


    shared_ptr<renderingEngine> const rE = make_shared<myRenderingEngine>(meshes,pPs, grid);

    shared_ptr<TimeUpdate> const update = make_shared<ExplicitTimeUpdate> (meshes,pPs,grid);

    shared_ptr<physicEngine> const  pE= make_shared<myPhysicEngine>(update);
    bool re_err = rE->init();

    if(re_err){
        return 1;
    }
    pE->init();
   double t = 0.0;
   double currentTime = glfwGetTime(); //gafferongames.com
   double accumulator = 0.0;
  while (rE->shouldClose()){
        //while(sim_t < static_fps){
        //timeStepEnd = timeStart;
        double newTime = glfwGetTime();
        double frameTime = newTime -currentTime;
        currentTime = newTime;
        accumulator += frameTime;
        while (accumulator >= dt){

        //std::cout << (timeStart - timeStepEnd) * 1000 << " ms for TimeStep."<<std::endl;

        pE->update(DT);
        accumulator -=dt;
        t+=dt;
        }
        //sim_t += dt;
        //}

        //timeEndPhysic = glfwGetTime();
        //std::cout << (timeEndPhysic - timeStart) * 1000 << " ms for Physic-Engine."<<std::endl;
        //timeStartRendering = glfwGetTime();
        rE->render();
        //timeEnd = glfwGetTime();
        //std::cout << (timeEnd - timeStartRendering) * 1000 << " ms for Rendering-Engine.\n"<<std::endl << std::flush;

    }
    rE->stop();

}

int main(){

    if(!launchSnow()){
        return 1;
    }

return 0;
}



