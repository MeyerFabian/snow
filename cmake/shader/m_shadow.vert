#version 330
layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 Texture;
uniform mat4 gMVP;

out vec2 Texture0;
void main(void)
{
    Texture0 = Texture;
    gl_Position = gMVP * vec4(Position.x,Position.y,Position.z,1.0);
}
