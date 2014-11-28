#version 440
layout (location = 0) in vec4 Position;

uniform mat4 gMVP;
out float z;
out float w;
void main(void)
{





    gl_Position = gMVP* vec4(Position.xyz,1.0) +vec4(0.0f,.0f,.0f,Position.w);

    z = Position.x;
    w = Position.w;
    gl_PointSize =  Position.w/((1.0e-6));
}
