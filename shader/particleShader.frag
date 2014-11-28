in float z;
in float w;
void main(void)
{

    gl_FragColor= vec4((z+3)/6,(z+3)/6,1.0-(z+3)/6,1.0);

    if(w < 0.000000001){
        discard;
    }

}
