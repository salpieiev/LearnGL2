const char *SpotLightVertexShader = STRINGIFY
(

attribute vec4 Position;
attribute vec4 SourceColor;
attribute vec3 Normal;

uniform mat4 Projection;
uniform mat4 Modelview;
uniform mat3 NormalMatrix;
uniform vec3 LightPosition;
uniform vec3 AmbientLight;

varying vec4 DestinationColor;

void main()
{
    vec3 lightDirection = LightPosition - Position.xyz;
    
    // Compute distance attenuation
    vec3 distanceAttenuation;
    distanceAttenuation.x = 1.0;
    distanceAttenuation.z = dot(lightDirection, lightDirection);
    distanceAttenuation.y = sqrt(distanceAttenuation.z);
    
    vec3 attFactors = vec3(0.05, 0.08, 0.02);
    float attenuationFactor = 1.0 / dot(distanceAttenuation, attFactors);
    
    // Normalize the light direction vector
    lightDirection = normalize(lightDirection);
    
    // Compute spot cutoff factor
    // Later!!!
    
    // Compute Color
    vec3 N = NormalMatrix * Normal;
    vec3 L = lightDirection;
    
    float df = max(0.0, dot(N, L));
    
    vec3 color = AmbientLight + df * SourceColor.rgb;
    color *= attenuationFactor;
    
    DestinationColor = vec4(color, 1.0);
    
    

    gl_Position = Projection * Modelview * Position;
}

);