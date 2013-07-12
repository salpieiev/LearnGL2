const char *ProjectiveTexturingLightingFragmentShader = STRINGIFY
(

uniform highp sampler2D u_sampler;

varying highp vec4 v_color;
varying highp vec3 v_normal;
varying highp vec3 v_lightDirection;
varying highp vec3 v_projectorDirection;
varying highp vec3 v_projectiveTexCoord;


void main()
{
    highp float df = max(0.0, dot(v_normal, v_lightDirection));
    highp vec4 color = df * v_color;
    
    if (v_projectiveTexCoord.x / v_projectiveTexCoord.z < 0.0) discard;
    if (v_projectiveTexCoord.y / v_projectiveTexCoord.z < 0.0) discard;
    if (v_projectiveTexCoord.x / v_projectiveTexCoord.z > 1.0) discard;
    if (v_projectiveTexCoord.y / v_projectiveTexCoord.z > 1.0) discard;
    
    highp vec4 projectorColor = texture2DProj(u_sampler, v_projectiveTexCoord);
    highp float dfp = max(0.0, dot(v_normal, v_projectorDirection));
    highp vec4 projector = 2.0 * dfp * projectorColor;

    gl_FragColor = /*color * */projector;
}


);