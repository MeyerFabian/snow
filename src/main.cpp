
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
using namespace  std;

double dt = 0.00001;
double sim_t = 0.0;
double static_fps = 0.01666666666;


int launchSnow(){
    shared_ptr<Grid>const  grid = make_shared<Grid >(GRID_DIM_X,GRID_DIM_Y,GRID_DIM_Z, GRID_SPACING, GRID_POS_X,GRID_POS_Y,GRID_POS_Z);


    shared_ptr<ParticleSystem> const pPs= make_shared<ParticleSystem > ();
    float xpos=-0.5f,ypos=0.5f,zpos=-0.5f;
    for(int x = 0; x < 32; x+=1){
        xpos += 0.1f;
        for(int y = 0; y < 32; y+=1){
            ypos += 0.1f;
            for(int z = 0; z < 32; z+=1){
                zpos += 0.1f;
                pPs->particles->push_back(Particle(Vector3f(xpos,ypos,zpos)));
            }
            zpos = -0.5f;
        }
        ypos = 0.5f;
    }

    shared_ptr<std::vector<Mesh> > const  meshes = make_shared<std::vector<Mesh> >();

    Mesh jeep;
    jeep.LoadMesh("model/jeep.obj");
    jeep.setPosition(-.54f,-0.25f,0.0f);
    jeep.setScale(0.003f,0.003f,0.003f);
    jeep.setRotation(0,0,0);
    Mesh quad;
    quad.LoadMesh("model/box.obj");
    quad.setPosition(0,-3.0f,0.0f);
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
    double timeEnd = 0;
    double timeStart=0;
    double timeStartRendering=0;
    double timeEndPhysic = 0;
    double timeStepEnd = 0;
  while (rE->shouldClose()){
        //while(sim_t < static_fps){
        //timeStepEnd = timeStart;
        //timeStart = glfwGetTime();
        //std::cout << (timeStart - timeStepEnd) * 1000 << " ms for TimeStep."<<std::endl;

        pE->update(dt);
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



