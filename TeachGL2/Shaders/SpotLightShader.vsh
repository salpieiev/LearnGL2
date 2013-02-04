const char *SpotLightVertexShader = STRINGIFY
(

attribute vec4 Position;
attribute vec4 SourceColor;
attribute vec3 Normal;
attribute vec3 LightPosition;

uniform mat4 Projection;
uniform mat4 Modelview;
uniform mat3 NormalMatrix;
uniform vec3 AmbientLight;

varying vec4 DestinationColor;

void main()
{
    vec3 N = NormalMatrix * Normal;
    vec3 L = normalize(LightPosition);
    
    float df = max(0.0, dot(N, L));
    
    vec3 color = AmbientLight + df * SourceColor.rgb;
    DestinationColor = vec4(color, 1.0);

    gl_Position = Projection * Modelview * Position;
}

);