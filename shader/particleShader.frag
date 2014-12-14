in float z;
in float w;
void main(void)
{

    gl_FragColor= vec4((z),(z),1.0-(z),1.0);

    if(w < 1e-9){
        discard;
    }

}
