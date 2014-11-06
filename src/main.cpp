
#include "myRenderingEngine.h"
#include "myPhysicEngine.h"
#include <iostream>
using namespace  std;
renderingEngine* rE;
physicEngine* pE;
double dt = 0.00001;
double sim_t = 0.0;
double static_fps = 0.01666666666;
int main(){

    rE = new myRenderingEngine();
    pE = new myPhysicEngine();
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
    delete rE;
return 0;
}


