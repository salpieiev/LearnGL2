const char *SpotLightFragmentShader = STRINGIFY
(

varying lowp vec4 DestinationColor;

void main()
{
    gl_FragColor = DestinationColor;
}

);