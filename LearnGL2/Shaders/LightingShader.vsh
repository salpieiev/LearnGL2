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
    vec3 N = u_normalMatrix * a_normal;
    vec3 L = /*u_normalMatrix * */normalize(a_position.xyz - u_lightPosition);
    vec3 E = /*u_normalMatrix * */normalize(a_position.xyz - u_eyePosition);
    L = normalize(vec3(0.25, 0.25, 1.0));
    E = normalize(vec3(0.0, 0.0, 1.0));
    vec3 H = normalize(L + E);
    float df = max(0.0, dot(N, L));
    float sf = max(0.0, dot(N, H));
    sf = pow(sf, u_shininess);
    
    vec3 color = u_ambientColor + df * a_color.xyz + sf * u_specularColor;
    v_color = vec4(color, 1.0);

    gl_Position = u_projection * u_modelview * a_position;
}

);