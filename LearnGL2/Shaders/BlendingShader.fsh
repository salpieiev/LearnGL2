const char *BlendingFragmentShader = STRINGIFY
(

varying highp vec4 v_color;

void main()
{
    gl_FragColor = v_color;
}

);