#version 330
layout (location = 0) in vec3 Position;

uniform mat4 gMVP;
void main(void)
{
    gl_Position = gMVP* vec4(Position,1.0);
}
