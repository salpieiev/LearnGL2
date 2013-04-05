const char *StencilVertexShader = STRINGIFY
(

attribute vec4 a_position;
attribute vec4 a_color;
attribute vec3 a_normal;

uniform mat4 u_projection;
uniform mat4 u_modelview;
uniform mat3 u_normalMatrix;
uniform vec3 u_lightPosition;
uniform vec3 u_ambientLight;

varying vec4 v_color;

void main()
{
    gl_Position = u_projection * u_modelview * a_position;
    
    vec3 N = u_normalMatrix * a_normal;
    vec3 L = normalize(u_lightPosition);
    float df = max(0.0, dot(N, L));
    
    vec3 diffuseColor = a_color.rgb;
    vec3 color = u_ambientLight + df * diffuseColor;
    
    v_color = vec4(color, 1);
}

);