#include "myphysicengine.h"

bool myPhysicEngine::init(){
     integration->init();
}
void myPhysicEngine::update(double dt){
    integration->update(dt);
}
