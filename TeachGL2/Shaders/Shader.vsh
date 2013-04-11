const char *VertexShader = STRINGIFY
(

attribute vec4 Position;
attribute vec4 SourceColor;

varying vec4 DestinationColor;

void main()
{
    DestinationColor = SourceColor;
    gl_Position = Position;
}

);