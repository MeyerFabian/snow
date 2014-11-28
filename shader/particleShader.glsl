#version 440

layout(points) in;
layout(points) out;
layout (max_vertices=1) out;
void main(void)
{


    vec3 pos = gl_in[0].gl_Position.xyz;
    gl_Position= vec4(pos,1.0f);
    //if(pos.w < 0.000000001){
    //    discard;
    //}
    //EmitVertex();
    //EndPrimitive();
}
