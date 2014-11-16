in float z;
void main(void)
{

    gl_FragColor= vec4((z+3)/6,(z+3)/6,1.0-(z+3)/6,1.0);
}
