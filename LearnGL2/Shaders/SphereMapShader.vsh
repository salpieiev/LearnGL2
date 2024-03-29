const char *SphereMapVertexShader = STRINGIFY
(

attribute vec4 Position;
attribute vec3 Normal;

uniform mat4 Projection;
uniform mat4 Modelview;
uniform mat3 NormalMatrix;
uniform vec3 EyePosition;

varying vec2 TextureCoord;


highp vec2 SphereMap(in highp vec3 position, in highp vec3 normal);


void main()
{
    gl_Position = Projection * Modelview * Position;
    
    // Compute eye direction in object space
    mediump vec3 EyeDir = /*NormalMatrix * */normalize(Position.xyz - EyePosition);
    
    TextureCoord = SphereMap(EyeDir, Normal);
}


highp vec2 SphereMap(in highp vec3 position, in highp vec3 normal)
{
    highp vec3 reflection = reflect(position, normal);
    highp float m = 2.0 * sqrt(reflection.x * reflection.x + reflection.y * reflection.y + (reflection.z + 1.0) * (reflection.z + 1.0));
    return vec2((reflection.x / m + 0.5), (reflection.y / m + 0.5));
}


);