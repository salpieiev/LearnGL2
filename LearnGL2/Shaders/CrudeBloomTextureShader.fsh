const char *CrudeBloomTextureFragmentShader = STRINGIFY
(

uniform highp sampler2D u_sampler;
uniform mediump float u_threshold;

varying highp vec2 v_texCoord;

const mediump vec3 Perception = vec3(0.299, 0.587, 0.114);

void main()
{
    mediump vec3 color = texture2D(u_sampler, v_texCoord).rgb;
    mediump float luminance = dot(Perception, color);
    gl_FragColor = (luminance > u_threshold) ? vec4(color, 1) : vec4(0);
}

);