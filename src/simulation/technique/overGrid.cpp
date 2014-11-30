#include "overGrid.h"
#include <iostream>
bool OverGrid::init(string cs){

    Technique::init();

    addShader(cs.c_str(),GL_COMPUTE_SHADER);

    finalize();

    return true;
}
