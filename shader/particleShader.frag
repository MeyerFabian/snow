in float z;
in float w;
void main(void)
{

    gl_FragColor= vec4((z-5.0),(z-5.0),1.0-(z-5.0),1.0);
/*
    if(w < 1e-6){
        discard;
    }
*/
}
