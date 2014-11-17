
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
using namespace  std;

double dt = 0.00001;
double sim_t = 0.0;
double static_fps = 0.01666666666;


int launchSnow(){
    shared_ptr<Grid>const  grid = make_shared<Grid >(GRID_DIM_X,GRID_DIM_Y,GRID_DIM_Z, GRID_SPACING, GRID_POS_X,GRID_POS_Y,GRID_POS_Z);


    shared_ptr<ParticleSystem> const pPs= make_shared<ParticleSystem > ();
    float xpos=-0.5f,ypos=0.5f,zpos=-0.5f;
    for(int x = 0; x < 128; x+=1){
        xpos += 0.01f;
        for(int y = 0; y < 64; y+=1){
            ypos += 0.01f;
            for(int z = 0; z < 128; z+=1){
                zpos += 0.01f;
                pPs->particles->push_back(Particle(Vector3f(xpos,ypos,zpos)));
            }
            zpos = -0.5f;
        }
        ypos = 0.5f;
    }

    shared_ptr<std::vector<Mesh> > const  meshes = make_shared<std::vector<Mesh> >();

    Mesh jeep;
    jeep.LoadMesh("model/jeep.obj");
    Mesh quad;
    quad.LoadMesh("model/quad2.obj");
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

  while (rE->shouldClose()){
        //while(sim_t < static_fps){

        pE->update(dt);
        //sim_t += dt;
        //}
        rE->render();
    }
    rE->stop();

}

int main(){

    if(!launchSnow()){
        return 1;
    }

return 0;
}



