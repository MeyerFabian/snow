#version 440
layout (location = 0) in vec4 Position;

uniform mat4 gMVP;
out float z;
void main(void)
{



    gl_Position = gMVP* vec4(Position.xyz,1.0);

    z = Position.x;
    gl_PointSize = Position.w/((1.0e-6)*64);
}
