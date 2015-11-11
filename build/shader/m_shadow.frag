in vec2 Texture0;
uniform sampler2D gSampler;
void main(void)
{
    float Depth = texture2D(gSampler,Texture0);
    gl_FragColor = vec4(Depth,Depth,Depth,1.0);
}
