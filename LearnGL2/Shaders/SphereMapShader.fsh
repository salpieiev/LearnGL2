const char *SphereMapFragmentShader = STRINGIFY
(

varying mediump vec2 TextureCoord;

uniform sampler2D Sampler;


void main()
{
    gl_FragColor = texture2D(Sampler, TextureCoord);
}

);