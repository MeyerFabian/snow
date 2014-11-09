#include "particletechnique.h"
#include <iostream>
ParticleTechnique::ParticleTechnique()
{
}
void ParticleTechnique::setWVP(const Matrix4f* MVP){
    glUniformMatrix4fv(ModelViewPersp,1,GL_TRUE,(const GLfloat*)MVP);
}
bool ParticleTechnique::init(string vs, string fs){

    Technique::init();

    addShader(vs.c_str(),GL_VERTEX_SHADER);
    addShader(fs.c_str(),GL_FRAGMENT_SHADER);

    finalize();

    ModelViewPersp = glGetUniformLocation(this->ShaderProgram,"gMVP");

    if (ModelViewPersp == INVALID_UNIFORM_LOCATION){
        return false;
    }
    return true;
}
