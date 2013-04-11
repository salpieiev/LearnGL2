const char *FragmentShader = STRINGIFY
(

precision mediump float;

varying lowp vec4 DestinationColor;

void main()
{
    gl_FragColor = DestinationColor;
}

);