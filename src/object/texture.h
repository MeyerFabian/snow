#ifndef TEXTURE_H
#define TEXTURE_H
#include "stb_image.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <string>
#include <iostream>
using namespace std;
class Texture
{
public:
    Texture(string filename);
    bool Load(GLenum textureType);
    void bind(GLenum texture);

    string filename;
private:
    GLuint id;
    GLenum texture;
    GLenum textureType;
};

#endif // TEXTURE_H
