const char *ProjectiveTexturingLightingVertexShader = STRINGIFY
(

attribute vec4 a_position;
attribute vec4 a_color;
attribute vec3 a_normal;

uniform mat4 u_projection;
uniform mat4 u_modelview;
uniform mat4 u_lightProjection;
uniform mat4 u_lightModelview;
uniform mat3 u_normalMatrix;
uniform mat3 u_biasMatrix;
uniform vec3 u_lightPosition;

varying vec4 v_color;
varying vec3 v_normal;
varying vec3 v_lightDirection;


void main()
{
    gl_Position = u_projection * u_modelview * a_position;

    v_color = a_color;
    v_normal = u_normalMatrix * a_normal;
    v_lightDirection = u_normalMatrix * normalize(u_lightPosition - a_position.xyz);
}


);