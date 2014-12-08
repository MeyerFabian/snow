#include "overGrid.h"
#include <iostream>
bool OverGrid::init(string cs){

    Technique::init();

    addShader(cs.c_str(),GL_COMPUTE_SHADER);

    finalize();

    dt =glGetUniformLocation(this->ShaderProgram, "dt");

    return true;
}
void OverGrid::setDt(const float deltat){
    glUniform1f(dt,deltat);
}
