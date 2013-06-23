const char *ProjectiveTexturingLightingVertexShader = STRINGIFY
(

attribute vec4 a_position;
attribute vec4 a_color;
attribute vec3 a_normal;

uniform mat4 u_projection;
uniform mat4 u_modelview;
uniform mat3 u_normalMatrix;
uniform vec3 u_lightPosition;

varying vec4 v_color;


void main()
{
    gl_Position = u_projection * u_modelview * a_position;
    
    vec3 lightDirection = normalize(u_lightPosition);
    vec3 N = u_normalMatrix * a_normal;
    vec3 L = u_normalMatrix * lightDirection;
    float df = max(0.0, dot(N, L));
    vec4 color = df * a_color;
    v_color = color;
}


);