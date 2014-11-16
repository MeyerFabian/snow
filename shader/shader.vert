#version 440
layout (location = 0) in vec4 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 Texture;
uniform mat4 gModel;
uniform mat4 gMVP;
uniform mat4 inverse;
uniform mat4 gLightMVP;
out vec4 Position0;
out vec4 Normal0;
out vec2 Texture0;
out vec4 LightSpacePos;
void main(void)
{
    gl_Position = gMVP* vec4(Position.xyz,1.0);
    Position0 = gModel* vec4(Position.xyz,1.0);
    LightSpacePos = gLightMVP * vec4(Position.xyz,1.0);
    Texture0 = Texture;
    Normal0 =(inverse* vec4(Normal, 0.0));
}
