
#include "rendering/myRenderingEngine.h"
#include "simulation/myPhysicEngine.h"
#include <iostream>
#include <memory>
#include "object/mesh.h"
#include <vector>
#include "object/grid.h"
#include "object/particlesystem.h"
using namespace  std;

double dt = 0.00001;
double sim_t = 0.0;
double static_fps = 0.01666666666;


int launchSnow(){
    Grid grid();

    shared_ptr<ParticleSystem> const pPs= make_shared<ParticleSystem > ();
    float xpos=-1.0f,ypos=-1.0f,zpos=-1.0f;
    for(int x = 0; x <= 100; x+=1){
        xpos += 0.01f;
        for(int y = 0; y <= 100; y+=1){
            ypos += 0.01f;
            for(int z = 0; z <= 100; z+=1){
                zpos += 0.01f;
                pPs->particles->push_back(Particle(Vector3f(xpos,ypos,zpos)));
            }
            zpos = -1.0f;
        }
        ypos = -1.0f;
    }

    shared_ptr<std::vector<Mesh> > const  meshes = make_shared<std::vector<Mesh> >();
    Mesh jeep;
    jeep.LoadMesh("model/jeep.obj");
    Mesh quad;
    quad.LoadMesh("model/quad2.obj");
     meshes->push_back(std::move(jeep));
    meshes->push_back(std::move(quad));

    shared_ptr<renderingEngine> const rE = make_shared<myRenderingEngine>(meshes,pPs);
    shared_ptr<physicEngine> const  pE= make_shared<myPhysicEngine>(meshes,pPs);
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



