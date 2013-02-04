const char *LightVertexShader = STRINGIFY
(

attribute vec4 Position;
attribute vec4 SourceColor;
attribute vec3 Normal;
attribute vec3 LightDirection;

uniform mat4 Projection;
uniform mat4 Modelview;
uniform mat3 NormalMatrix;
uniform vec3 AmbientLight;
uniform vec3 SpecularLight;
uniform float Shininess;

varying vec4 DestinationColor;

void main()
{
    gl_Position = Projection * Modelview * Position;
    
    vec3 N = NormalMatrix * Normal;
    vec3 L = NormalMatrix * LightDirection;
    vec3 E = vec3(0, 0, 1);
    vec3 H = normalize(L + E);
    float df = max(0.0, dot(N, L));
    float sf = max(0.0, dot(N, H));
    sf = pow(sf, Shininess);
    
    vec3 diffuseColor = vec3(SourceColor.x, SourceColor.y, SourceColor.z);
    vec3 color = AmbientLight + df * diffuseColor + sf * SpecularLight;
    DestinationColor = vec4(color, 1);
}

);