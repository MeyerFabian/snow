#include "myphysicengine.h"

bool myPhysicEngine::init(){
     integration->init();
	 return true;
}
void myPhysicEngine::update(double dt){
    integration->update(dt);
}
