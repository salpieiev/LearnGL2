const char *FogFragmentShader = STRINGIFY
(

varying mediump vec4 v_color;


void main()
{
    gl_FragColor = v_color;
}


);