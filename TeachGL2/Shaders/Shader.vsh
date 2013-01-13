static const char * const VertexShader = STRINGIFY
(

attribute vec4 vPosition;

void main()
{
    gl_Position = vPosition;
}

);