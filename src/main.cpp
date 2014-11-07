
#include "rendering/myRenderingEngine.h"
#include "simulation/myPhysicEngine.h"
#include <iostream>
#include <memory>
#include "object/mesh.h"
#include <vector>
using namespace  std;

double dt = 0.00001;
double sim_t = 0.0;
double static_fps = 0.01666666666;


int launchSnow(){
    shared_ptr<std::vector<Mesh> > meshes = make_shared<std::vector<Mesh> >();

    Mesh jeep;
    jeep.LoadMesh("model/jeep.obj");
    Mesh quad;
    quad.LoadMesh("model/quad2.obj");

    meshes->push_back(std::move(jeep));
    meshes->push_back(std::move(quad));
    shared_ptr<renderingEngine> rE = make_shared<myRenderingEngine>();
    shared_ptr<physicEngine> pE= make_shared<myPhysicEngine>();
    bool re_err = rE->init(meshes);

    if(re_err){
        return 1;
    }
    pE->init();

  while (rE->shouldClose()){
        //while(sim_t < static_fps){
        pE->update(meshes,dt);
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



