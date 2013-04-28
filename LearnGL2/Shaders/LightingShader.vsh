const char *LightingVertexShader = STRINGIFY
(

attribute vec4 a_position;
attribute vec4 a_color;
attribute vec3 a_normal;

uniform mat4 u_projection;
uniform mat4 u_modelview;
uniform mat3 u_normalMatrix;
uniform vec3 u_lightPosition;
uniform vec3 u_eyePosition;
uniform vec3 u_ambientColor;
uniform vec3 u_specularColor;
uniform float u_shininess;

varying vec4 v_color;

void main()
{
    vec3 pos = (u_modelview * a_position).xyz;
    
    vec3 N = u_normalMatrix * a_normal;
    vec3 L = normalize(u_lightPosition - pos);
    vec3 E = normalize(u_eyePosition - pos);
    vec3 H = normalize(L + E);
    float df = max(0.0, dot(N, L));
    float sf = max(0.0, dot(N, H));
    sf = pow(sf, u_shininess);
    
    vec3 color = u_ambientColor + df * a_color.xyz + sf * u_specularColor;
    v_color = vec4(color, 1.0);

    gl_Position = u_projection * u_modelview * a_position;
}

);