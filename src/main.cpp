
#include "rendering/myRenderingEngine.h"
#include "simulation/myPhysicEngine.h"
#include <iostream>
#include <memory>
using namespace  std;

double dt = 0.00001;
double sim_t = 0.0;
double static_fps = 0.01666666666;

int launchSnow(){
    shared_ptr<renderingEngine> rE = make_shared<myRenderingEngine>();
    shared_ptr<physicEngine> pE= make_shared<myPhysicEngine>();

    bool re_err = rE->init();
    if(re_err){
        return 1;
    }
    pE->init();
    while (rE->shouldClose()){
        while(sim_t < static_fps){
        pE->update(dt);
        sim_t += dt;
        }
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



