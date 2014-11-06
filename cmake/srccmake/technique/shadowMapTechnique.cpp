#include "shadowMapTechnique.h"

ShadowMapTechnique::ShadowMapTechnique()
{

}
bool ShadowMapTechnique::init(string vs, string fs){
    Technique::init();

    addShader(vs.c_str(),GL_VERTEX_SHADER);
    addShader(fs.c_str(),GL_FRAGMENT_SHADER);

    finalize();

    gMVP = glGetUniformLocation(this->ShaderProgram, "gMVP");
    gSampler = glGetUniformLocation(this->ShaderProgram, "gSampler");

    if (gMVP == INVALID_UNIFORM_LOCATION ||
        gSampler == INVALID_UNIFORM_LOCATION) {
        return false;
    }
}

void ShadowMapTechnique::setMVP(const Matrix4f* m){
    glUniformMatrix4fv(gMVP, 1, GL_TRUE, (const GLfloat *) m);
}
void ShadowMapTechnique::setTex(unsigned int texture){
    glUniform1i(gSampler,texture);
}
