#ifndef SHADOWMAPBUFFEROBJECT_H
#define SHADOWMAPBUFFEROBJECT_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <stdio.h>
class ShadowMapBufferObject
{
public:
    ShadowMapBufferObject();
    ~ShadowMapBufferObject();

    bool Init(unsigned int WindowWidth, unsigned int WindowHeight);
    void BindForWriting();
    void BindForReading(GLenum TextureUnit);
private:
    GLuint m_fbo;
    GLuint m_shadowMap;
};

#endif // SHADOWMAPBUFFEROBJECT_H
