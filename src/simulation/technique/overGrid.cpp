#include "overGrid.h"
#include <iostream>
bool OverGrid::init(string cs){

    Technique::init();

    addShader(cs.c_str(),GL_COMPUTE_SHADER);

    finalize();

    dt =glGetUniformLocation(this->ShaderProgram, "dt");
    critComp =glGetUniformLocation(this->ShaderProgram, "critComp");
    critStretch =glGetUniformLocation(this->ShaderProgram, "critStretch");

    return true;
}
void OverGrid::setDt(const float deltat){
    glUniform1f(dt,deltat);
}
void OverGrid::setCritComp(){
glUniform1f(critComp,CRIT_COMPRESSION);
}

void OverGrid::setCritStretch(){
glUniform1f(critStretch,CRIT_STRETCH);
}
