const char *CrudeBloomTextureFragmentShader = STRINGIFY
(

uniform highp sampler2D u_sampler;

varying highp vec2 v_texCoord;

void main()
{
    gl_FragColor = texture2D(u_sampler, v_texCoord);
}

);