#include "shader.h"


Shader::Shader(string vs,string fs)
{

    ShaderProgram= glCreateProgram();

    if (ShaderProgram == 0) {
        fprintf(stderr, "Error creating shader program\n");
        exit(1);
    }


    this->addShader(ShaderProgram, vs.c_str(),GL_VERTEX_SHADER);
    this->addShader(ShaderProgram, fs.c_str(),GL_FRAGMENT_SHADER);


    this->useShaderProgram(ShaderProgram);

}

GLuint Shader::getShaderProgram(){
    return this->ShaderProgram;
}
void Shader::plugShader(){
    glUseProgram(ShaderProgram);
}

void Shader::addShader(GLuint ShaderProgram, const char* pShaderText ,GLenum ShaderType){

    GLuint ShaderObj = glCreateShader(ShaderType);
    const GLchar* p[1];
    p[0] = pShaderText;
    GLint Lengths[1];
    Lengths[0] = strlen(pShaderText);
    glShaderSource(ShaderObj,1,p,Lengths);
    glCompileShader(ShaderObj);
    GLint success;
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
        exit(1);
    }
    glAttachShader(ShaderProgram,ShaderObj);
}


void Shader::useShaderProgram(GLenum ShaderProgram){
    GLint Success=0;
    GLchar ErrorLog[1024] = { 0 };
    glLinkProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
    if (Success == 0) {
            glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
            fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
    }

    glValidateProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
    if (!Success) {
            glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
            fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
            exit(1);
    }
}


