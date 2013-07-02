const char *ProjectiveTexturingLightingFragmentShader = STRINGIFY
(

varying highp vec4 v_color;
varying highp vec3 v_normal;
varying highp vec3 v_lightDirection;


void main()
{
    highp float df = max(0.0, dot(v_normal, v_lightDirection));
    highp vec4 color = df * v_color;
    
    gl_FragColor = color;
}


);