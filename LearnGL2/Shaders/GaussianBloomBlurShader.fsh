const char *GaussianBloomBlurFragmentShader = STRINGIFY
(

uniform sampler2D u_sampler;
uniform mediump float u_coefficients[3];
uniform mediump vec2 u_offset;

varying mediump vec2 v_texCoord;

void main()
{
    mediump vec3 A = u_coefficients[0] * texture2D(u_sampler, v_texCoord - u_offset).rgb;
    mediump vec3 B = u_coefficients[1] * texture2D(u_sampler, v_texCoord).rgb;
    mediump vec3 C = u_coefficients[2] * texture2D(u_sampler, v_texCoord + u_offset).rgb;
    mediump vec3 color = A + B + C;
    gl_FragColor = vec4(color, 1.0);
}

);