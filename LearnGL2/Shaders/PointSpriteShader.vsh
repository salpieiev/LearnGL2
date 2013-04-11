const char *PointSpriteVertexShader = STRINGIFY
(

attribute vec4 Position;

void main()
{
    gl_Position = Position;
    gl_PointSize = 30.0;
}

);