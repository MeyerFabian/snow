#include "texture.hpp"

Texture::Texture(string filename)
{
       this->filename =filename;


}
void Texture::bind(GLenum TextureUnit){
    glActiveTexture(TextureUnit);
    glBindTexture(textureType, this->id);
}
bool Texture::Load(GLenum textureType){
    int    ImageX,
           ImageY,
           ImageComponents;
    GLubyte *ImageData = stbi_load(filename.c_str(), &ImageX, &ImageY, &ImageComponents, 4);
    if(ImageData == NULL){
        return false;
    }
    std::cout << "Loaded Texture:" << filename<<std::endl;
    this->textureType = textureType;
    glGenTextures(1, &this->id);
    glBindTexture(textureType, this->id);
    glTexImage2D(textureType,0,GL_RGBA8,ImageX, ImageY,0,GL_RGBA,GL_UNSIGNED_BYTE,ImageData);

    glTexParameterf(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    return true;
}
