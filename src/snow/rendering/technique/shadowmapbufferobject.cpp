#include "shadowmapbufferobject.hpp"

ShadowMapBufferObject::ShadowMapBufferObject()
{
    m_fbo = 0;
    m_shadowMap = 0;
}

ShadowMapBufferObject::~ShadowMapBufferObject(){
    if (m_fbo != 0) {
        glDeleteFramebuffers(1, &m_fbo);
    }

    if (m_shadowMap != 0) {
        glDeleteTextures(1, &m_shadowMap);
    }
}


bool ShadowMapBufferObject::Init(unsigned int WindowWidth, unsigned int WindowHeight){
    glGenFramebuffers(1,&m_fbo);

    glGenTextures(1,&m_shadowMap);

    glBindTexture(GL_TEXTURE_2D, m_shadowMap);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT,WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT,GL_FLOAT, NULL);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);

    glBindFramebuffer(GL_FRAMEBUFFER,m_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,m_shadowMap,0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(Status!=GL_FRAMEBUFFER_COMPLETE){
        printf("FB error, status: 0x%x\n",Status);
        return false;
    }
    return true;
}

void ShadowMapBufferObject::BindForWriting(){
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
}
void ShadowMapBufferObject::BindForReading(GLenum TextureUnit){
    glActiveTexture(TextureUnit);
    glBindTexture(GL_TEXTURE_2D, m_shadowMap);
}

