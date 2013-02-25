const char *MultitextureFragmentShader = STRINGIFY
(

precision mediump float;

uniform sampler2D u_baseSampler;
uniform sampler2D u_lightSampler;

varying mediump vec2 v_texCoord;

void main()
{
    vec4 baseColor = texture2D(u_baseSampler, v_texCoord);
    vec4 lightColor = texture2D(u_lightSampler, v_texCoord);
    
    gl_FragColor = baseColor * (lightColor + 0.25);
}

);