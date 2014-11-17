#version 440
layout (location = 0) in vec4 Position;

uniform mat4 gMVP;
out float z;
void main(void)
{
/*
    if(Position.w < 0.001){
        gl_Position=vec4(0,0,-500,1.0);
    }else
    {
*/
    gl_Position = gMVP* vec4(Position.xyz,1.0);
 //   }
    z = Position.z;
    gl_PointSize =  0+ Position.w/(1.0e-1);
}
