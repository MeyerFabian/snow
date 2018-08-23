#version 440
layout (location = 0) in vec4 Position;
layout (location = 1) in int mass;
uniform mat4 gMVP;
out float z;
out float w;
void main(void)
{





    gl_Position = gMVP* vec4(Position.xyz,1.0);

    z = Position.x;
    w = float(Position.w)/1000000.0f;
    gl_PointSize = 1;
}
