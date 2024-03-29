const char *RenderToTextureFragmentShader = STRINGIFY
(
 
 varying highp vec4 DestinationColor;
 varying mediump vec2 TextureCoordOut;
 
 uniform sampler2D Sampler;
 
 void main()
{
    gl_FragColor = texture2D(Sampler, TextureCoordOut) * DestinationColor;
}
 
 );