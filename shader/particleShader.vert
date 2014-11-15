#version 430
layout (location = 0) in vec4 Position;

uniform mat4 gMVP;
void main(void)
{
    gl_Position = gMVP* vec4(Position.xyz,1.0);

    gl_PointSize = 1;
}
