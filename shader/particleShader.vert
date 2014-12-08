#version 440
layout (location = 0) in vec4 Position;

uniform mat4 gMVP;
out float z;
out float w;
void main(void)
{





    gl_Position = gMVP* vec4(Position.xyz,1.0);

    z = Position.x;
    w = Position.w;
    gl_PointSize = 1;
}
