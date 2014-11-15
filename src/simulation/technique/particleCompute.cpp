#include "particleCompute.h"
bool ParticleCompute::init(string cs){

    Technique::init();

    addShader(cs.c_str(),GL_COMPUTE_SHADER);

    finalize();

    gGridPos = glGetUniformLocation(this->ShaderProgram, "gGridPos");

    if (gGridPos == INVALID_UNIFORM_LOCATION){
        return false;
    }
    return true;
}
void ParticleCompute::setGridPos(const float gridPosx,const float gridPosy,const float gridPosz){
    glUniform3f(gGridPos, gridPosx, gridPosy, gridPosz);
}
