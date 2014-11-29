#include "resetGrid.h"
#include <iostream>
bool ResetGrid::init(string cs){

    Technique::init();

    addShader(cs.c_str(),GL_COMPUTE_SHADER);

    finalize();

    return true;
}
