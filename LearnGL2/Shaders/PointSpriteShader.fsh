const char *PointSpriteFragmentShader = STRINGIFY
(

uniform sampler2D Sampler;

void main()
{
    gl_FragColor = texture2D(Sampler, gl_PointCoord);
}

);