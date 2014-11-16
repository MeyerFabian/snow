#include "particleCompute.h"
#include <iostream>
bool ParticleCompute::init(string cs){

    Technique::init();

    addShader(cs.c_str(),GL_COMPUTE_SHADER);

    finalize();

    gGridPos = glGetUniformLocation(this->ShaderProgram, "gGridPos");
    gGridDim = glGetUniformLocation(this->ShaderProgram, "gGridDim");
    dt = glGetUniformLocation(this->ShaderProgram, "dt");
    h =glGetUniformLocation(this->ShaderProgram, "gridSpacing");
    if (gGridPos == INVALID_UNIFORM_LOCATION ||
        gGridDim == INVALID_UNIFORM_LOCATION ||
        dt == INVALID_UNIFORM_LOCATION ||
        h == INVALID_UNIFORM_LOCATION){
        return false;
    }
    return true;
}
void ParticleCompute::setGridPos(const float gridPosx,const float gridPosy,const float gridPosz){
    glUniform3f(gGridPos, gridPosx, gridPosy, gridPosz);
}
void ParticleCompute::setGridDim(const int gridDimx,const int gridDimy,const int gridDimz){
    glUniform3i(gGridDim, gridDimx, gridDimy, gridDimz);
}
void ParticleCompute::setdt(const float deltatime){
    glUniform1f(dt, deltatime);
}
void ParticleCompute::setGridSpacing (const float spacing){
    glUniform1f(h,spacing);
}
