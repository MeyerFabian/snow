#ifndef SHADER_H
#define SHADER_H
#include "glew.h"

#include <string>
#include <string.h>
#include <fstream>
using namespace std;

class Shader
{
private:
    GLuint ShaderProgram;
public:

    Shader(string vs,string fs);
    GLuint getShaderProgram();
    void plugShader();
    void addShader(GLuint ShaderProgram, const char* pShaderText ,GLenum ShaderType);
    void useShaderProgram(GLenum ShaderProgram);
};



#endif // SHADER_H
