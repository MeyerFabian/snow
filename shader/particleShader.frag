in float z;
in float w;
void main(void)
{
    gl_FragColor.rgba= vec4(1.0-gl_PointCoord.t*0.4,1.0f-gl_PointCoord.t*0.20,1.0,1.0);

    if(w < 1e-6 || (gl_PointCoord.t*2.0-1.0)*(gl_PointCoord.t*2.0-1.0)+ (gl_PointCoord.s*2.0-1.0)*(gl_PointCoord.s*2.0-1.0)>1.0f){
        discard;
    }

}
